#include <functional>
#include <vector>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <random>
#include "fastEAlgs.h"
#include "logger.h"
#include <mutex>
#include <unordered_map>
#include <chrono>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>
#include <boost/dynamic_bitset.hpp>

const uint N_BEGIN = 100;
const uint N_END = 1000;
const uint N_STEP = 100;

const uint LAMBDA_BEGIN = 2;
const uint LAMBDA_END = 10;
const uint LAMBDA_STEP = 1;

const uint TESTS = 100;

using uint = uint32_t;

struct ResultsTable
{
    ResultsTable(const std::string &fileName, uint32_t offset) : toPrintNextLambda(LAMBDA_BEGIN),
        toPrintNextN(N_BEGIN), fileName(fileName), fout(fileName), offset(offset)
    {
        fout << std::setw(offset) << "lambda" << std::setw(offset) << "n" << std::setw(
                 offset) << "function evaluations" << std::setw(offset) << "standard deviation" << std::endl;
        fout << std::endl;
    }

    void add(uint lambda, uint n, uint evoluations, double deviation)
    {
        data_mtx.lock();
        results[lambda][n] = std::make_pair(evoluations, deviation);
        LOG("Done for lambda = ", lambda, ", n = ", n, ", for line: ", fileName);
        data_mtx.unlock();
    }

    void printResults()
    {
        data_mtx.lock();
        for (uint &i = toPrintNextLambda; i <= LAMBDA_END; i += LAMBDA_STEP)
        {
            if (toPrintNextN > N_END) toPrintNextN = N_BEGIN;
            if (!results.count(i)) break;
            bool ok = false;
            for (uint &j = toPrintNextN; j <= N_END; j += N_STEP)
            {
                ok = false;
                if (!results[i].count(j)) break;
                ok = true;
                print(i, j, results[i][j].first, results[i][j].second);
            }
            if (!ok) break;
            print();
        }
        data_mtx.unlock();
    }

    ~ResultsTable()
    {
        fout.close();
    }
private:

    void print(uint32_t lambda, uint32_t n, uint32_t ans, double deviation)
    {
        fout << std::setw(offset) << lambda << std::setw(offset) << n << std::setw(
                 offset) << ans << std::setw(offset) << deviation << std::endl;
    }

    void print()
    {
        fout << std::endl;
    }

    uint toPrintNextLambda;
    uint toPrintNextN;

    std::string fileName;
    std::ofstream fout;
    const uint32_t offset;
    std::mutex data_mtx;
    /* results[lambda][n] = <evaluations, deviation> */
    std::unordered_map <uint, std::unordered_map<uint, std::pair<uint, double>>> results;
};


void generate(boost::dynamic_bitset<> &x, std::mt19937 &engine)
{
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = static_cast<bool>(dis(engine));
}

using oneMax_t = uint32_t (const boost::dynamic_bitset<> &, uint32_t);

inline double getStandartDeviation(uint32_t ans, const std::vector<double> &t)
{
    double t_av = static_cast<double>(ans);
    double s = 0;
    double n = static_cast<double>(t.size());
    for (auto ti : t)
    {
        s += (ti - t_av) * (ti - t_av);
    }
    s /= n;
    return sqrt(s);
}

void average(oneMax_t *f, uint32_t n, uint32_t lambda, ResultsTable *table)
{
    uint32_t ans = 0;
    std::vector<double> t;
    std::random_device rd;
    std::mt19937 gen(rd());
    boost::dynamic_bitset<> offs(n);
    for (uint32_t test = 0; test < TESTS; ++test)
    {
        generate(offs, gen);
        uint32_t ti = f(offs, lambda);
        #ifndef NDEBUG
        assert(offs.count() == offs.size());
        #endif
        ans += ti;
        t.push_back(static_cast<double>(ti));
    }
    ans /= TESTS;
    table->add(lambda, n, ans, getStandartDeviation(ans, t));
    table->printResults();
}

void pool_all(const std::vector<std::pair<oneMax_t *, ResultsTable *>> &fts, size_t threadsAmount)
{
    boost::asio::thread_pool pool(threadsAmount);
    for (const auto &f : fts)
    {
        for (uint lambda = LAMBDA_BEGIN; lambda <= LAMBDA_END; lambda += LAMBDA_STEP)
        {
            for (uint i = N_BEGIN; i <= N_END; i += N_STEP)
            {
                auto task = std::bind(average, f.first, i, lambda, f.second);
                boost::asio::post(pool, task);
            }
        }
    }
    pool.join();
}

int main()
{
    auto start = std::chrono::steady_clock::now();

    ResultsTable table("resultsStatic.txt", 25);
    ResultsTable tableAdj("resultsAdjusting.txt", 25);

    std::vector<std::pair<oneMax_t *, ResultsTable *>> fts =
    {
        {&staticMutationProbability::fast::oneMax, &table},
        {&adjustingMutationProbabilityWithTwoOffsprings::fast::oneMax, &tableAdj}
    };

    pool_all(fts, 8);

    table.printResults();
    tableAdj.printResults();

    auto end = std::chrono::steady_clock::now();
    LOG("time : ", std::chrono::duration <double, std::milli> (end - start).count() / 1000., " secs");

    return 0;
}
