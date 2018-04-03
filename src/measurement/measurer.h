#ifndef MEASURER_H
#define MEASURER_H

#include "configParser.h"
#include <boost/dynamic_bitset.hpp>
#include <random>
#include "onePlusLambdaSolver.h"
#include "resultsTablePrinter.h"

class Measurer
{
    const ParsedParams params;
    void generate(boost::dynamic_bitset<> &x, std::mt19937 &engine) const;
    double getStandartDeviation(uint32_t ans, const std::vector<double> &t) const;
    void average(const std::shared_ptr<OnePlusLambdaSolver> &solver_ptr, uint32_t n, uint32_t lambda,
                 ResultsTablePrinter *table) const;
    void pool_all(const std::vector <
                  std::pair<std::shared_ptr<OnePlusLambdaSolver>, ResultsTablePrinter * >> &fts,
                  size_t threadsAmount) const;

    const uint32_t N_BEGIN;
    const uint32_t N_END;
    const uint32_t N_STEP;
    const uint32_t LAMBDA_BEGIN;
    const uint32_t LAMBDA_END;
    const uint32_t LAMBDA_STEP;
    const uint32_t TESTS;

public:
    Measurer(const ParsedParams &params) : params(params),
        N_BEGIN(params.n[0]), N_END(params.n[1]), N_STEP(params.n[2]),
        LAMBDA_BEGIN(params.lambda[0]), LAMBDA_END(params.lambda[1]), LAMBDA_STEP(params.lambda[2]),
        TESTS(params.tests)
    {}
    void run();
};

#endif // MEASURER_H
