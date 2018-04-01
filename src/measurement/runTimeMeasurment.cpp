#include <functional>
#include <vector>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include "logger.h"
#include <chrono>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <boost/dynamic_bitset.hpp>
#include "resultsTable.h"
#include "oneMaxSolver.h"
#include "staticParamsSolver.h"
#include "adjustingParamsSolver.h"

using uint = uint32_t;

const uint N_BEGIN = 10000;
const uint N_END = 100000;
const uint N_STEP = 10000;

const uint LAMBDA_BEGIN = 800;
const uint LAMBDA_END = 800;
const uint LAMBDA_STEP = 1;

const uint TESTS = 10;

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

void average(const std::shared_ptr<OnePlusLambdaSolver> &solver_ptr, uint32_t n, uint32_t lambda,
             ResultsTable *table)
{
    uint32_t ans = 0;
    std::vector<double> t;
    std::random_device rd;
    std::mt19937 gen(rd());
    boost::dynamic_bitset<> offs(n);
    for (uint32_t test = 0; test < TESTS; ++test)
    {
        generate(offs, gen);
        OneMaxOffspring x(offs, probability_t(1. / static_cast<double>(offs.size())));
        uint32_t ti = solver_ptr->solve(x, lambda);
        ans += ti;
        t.push_back(static_cast<double>(ti));
    }
    ans /= TESTS;
    table->add(lambda, n, ans, getStandartDeviation(ans, t));
    table->printResults();
}

void pool_all(const std::vector <
              std::pair<std::shared_ptr<OnePlusLambdaSolver>, ResultsTable * >> &fts,
              size_t threadsAmount)
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

    ResultsTable tableStatic("resultsStatic.txt", 25);
    ResultsTable tableAdjTwo("resultsAdjusting.txt", 25);
    ResultsTable tableAdjThree("resultsSeftAdjustingThree.txt", 25);

    std::vector<std::pair<std::shared_ptr<OnePlusLambdaSolver>, ResultsTable *>> fts =
    {
//        {std::make_shared<StaticParamsSolver>(StaticParamsSolver(std::vector<double>())), &tableStatic},
//        {std::make_shared<AdjustingParamsSolver<2>>(AdjustingParamsSolver<2>({0.5, 2.})), &tableAdjTwo},
        {std::make_shared<AdjustingParamsSolver<3>>(AdjustingParamsSolver<3>({2., 1., 0.5})), &tableAdjThree}
    };

    pool_all(fts, 8);

    for (const auto &ft : fts) ft.second->printResults();

    auto end = std::chrono::steady_clock::now();
    LOG("time : ", std::chrono::duration <double, std::milli> (end - start).count() / 1000., " secs");

    return 0;
}
