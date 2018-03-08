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
#include <thread>
#include <unordered_map>
#include <chrono>

const uint N_BEGIN = 100;
const uint N_END = 1000;
const uint N_STEP = 100;

const uint LAMBDA_BEGIN = 2;
const uint LAMBDA_END = 10;
const uint LAMBDA_STEP = 1;

const uint TESTS = 100;

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

using uint = uint32_t;
struct ResultsTextTable;

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


void generate(std::vector<bool> &x)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = static_cast<bool>(dis(gen));
}

using oneMax_t = uint32_t (const std::vector<bool> &, uint32_t);

uint32_t average(oneMax_t *f, uint32_t n, uint32_t lambda, ResultsTable *table)
{
    uint32_t ans = 0;
    std::vector<double> t;
    for (uint32_t test = 0; test < TESTS; ++test)
    {
        std::vector<bool> offs(n, false);
        generate(offs);
        uint32_t ti = f(offs, lambda);
        ans += ti;
        t.push_back(static_cast<double>(ti));
    }
    ans /= TESTS;
    table->add(lambda, n, ans, getStandartDeviation(ans, t));
    table->printResults();
    return ans;
}

void measure(oneMax_t *f, ResultsTable &table, uint32_t lambda, const char *nameOfGraphLine,
             size_t threadsAmount = 0)
{
    LOG("\n");
    LOG("Started line: ", nameOfGraphLine);
    LOG("\n");

    if (threadsAmount == 0)
    {
        for (uint i = N_BEGIN; i <= N_END; i += N_STEP)
            average(f, i, lambda, &table);
        LOG("Done for lambda = ", lambda, ", for line: ", nameOfGraphLine);
    }
    else
    {
        std::vector<std::thread> ths(threadsAmount);
        uint curTask = N_BEGIN;
        while (curTask <= N_END)
        {
            size_t buzy = 0;
            for (auto &t : ths)
            {
                if (curTask > N_END) break;
                t = std::thread(average, f, curTask, lambda, &table);
                ++buzy;
                curTask += N_STEP;
            }
            for (size_t i = 0; i != buzy; ++i)
                ths[i].join();
        }
        LOG("Done for lambda = ", lambda, ", for line: ", nameOfGraphLine);
    }
}

int main()
{
    auto start = std::chrono::steady_clock::now();

    ResultsTable table("resultsStatic.txt", 25);
    ResultsTable tableAdj("resultsAdjusting.txt", 25);

    for (uint32_t lambda = LAMBDA_BEGIN; lambda <= LAMBDA_END; lambda += LAMBDA_STEP)
    {
        measure(&staticMutationProbability::fast::oneMax, table, lambda, "Static Mutation Probability", 10);
        measure(&adjustingMutationProbabilityWithTwoOffsprings::fast::oneMax, tableAdj, lambda,
                "Adjusting Mutation Probability", 10);
    }

    table.printResults();
    tableAdj.printResults();

    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    LOG("time : ", std::chrono::duration <double, std::milli> (diff).count() / 1000., " secs");

    return 0;
}
