#include "measurer.h"
#include "adjustingParamsSolver.h"
#include "algorithmTags.h"
#include "internalReporter.h"
#include "logger.h"
#include "resultsTablePrinter.h"
#include "staticParamsSolver.h"
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/dynamic_bitset.hpp>
#include <memory>
#include <onePlusLambdaSolver.h>
#include <solversFabric.h>

void Measurer::generate(boost::dynamic_bitset<> &x, std::mt19937 &engine)
{
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = static_cast<bool>(dis(engine));
}

double Measurer::getStandartDeviation(uint64_t ans, const std::vector<double> &t)
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

void Measurer::average(AlgorithmsTags solver_tag, uint32_t n, uint32_t lambda,
                       ResultsTablePrinter *table)
{
    uint64_t ans = 0;
    std::vector<double> t;
    std::random_device rd;
    std::mt19937 gen(rd());
    boost::dynamic_bitset<> offs(n);
    std::shared_ptr<OnePlusLambdaSolver> solver_ptr = fabric.create_solver(solver_tag, lambda, n);
    for (uint32_t test = 0; test < TESTS; ++test)
    {
        generate(offs, gen);
        OneMaxOffspring x(offs, probability_t(1. / static_cast<double>(offs.size())));
        fs_lock.lock();
        auto reporter_ptr =
#ifdef ENABLE_INTERNAL_INFO
            Reporter::createReporter(solver_tag, lambda, n, solver_ptr->get_params_ptr());
#else
            nullptr;
#endif
        fs_lock.unlock();
        uint64_t ti = solver_ptr->solve(x, reporter_ptr);
        ans += ti;
        t.push_back(static_cast<double>(ti));
    }
    ans /= TESTS;
    table->add(lambda, n, ans, getStandartDeviation(ans, t));
    table->printResults();
    //    complete(id, table);
}

void Measurer::pool_all(
    const std::vector<std::pair<AlgorithmsTags, std::shared_ptr<ResultsTablePrinter>>> &fts,
    size_t threadsAmount)
{
    boost::asio::thread_pool pool(threadsAmount);
    for (const auto &f : fts)
    {
        for (uint lambda = LAMBDA_BEGIN; lambda <= LAMBDA_END; lambda += LAMBDA_STEP)
        {
            for (uint i = N_BEGIN; i <= N_END; i += N_STEP)
            {
                auto task = std::bind(&Measurer::average, this, f.first, i, lambda, f.second.get());
                boost::asio::post(pool, task);
            }
        }
    }
    pool.join();
}

void Measurer::run()
{
    auto start = std::chrono::steady_clock::now();

    LOG("Used params:\n", *params.get());

    SolversFabric fabric(params);
    auto fts = fabric.getAlgos();

    pool_all(fts, static_cast<size_t>(params->amountOfThreads));

    for (const auto &ft : fts)
        ft.second->printResults();

    auto end = std::chrono::steady_clock::now();
    LOG("time : ", std::chrono::duration<double, std::milli>(end - start).count() / 1000., " secs");
}

std::shared_ptr<ResultsTable> Measurer::runOneAlg()
{
    auto start = std::chrono::steady_clock::now();

    LOG("Used params:\n", *params.get());

    auto fts = fabric.getAlgos();
    SolversFabric::AlgTablePtrsVector anyAlg = {*fts.begin()};

    pool_all(anyAlg, 8);

    for (const auto &ft : fts)
        ft.second->printResults();

    auto end = std::chrono::steady_clock::now();
    LOG("time : ", std::chrono::duration<double, std::milli>(end - start).count() / 1000., " secs");
    return fts.begin()->second;
}
