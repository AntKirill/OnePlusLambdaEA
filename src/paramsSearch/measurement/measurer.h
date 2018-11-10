#ifndef MEASURER_H
#define MEASURER_H

#include "configParser.h"
#include <boost/dynamic_bitset.hpp>
#include <random>
#include "onePlusLambdaSolver.h"
#include "resultsTablePrinter.h"
#include "solversFabric.h"
#include "algorithmTags.h"

class Measurer
{
    const std::shared_ptr<const ParsedParams> params;
    void generate(boost::dynamic_bitset<> &x, std::mt19937 &engine);
    double getStandartDeviation(uint32_t ans, const std::vector<double> &t);
    void average(AlgorithmsTags solver_ptr, uint32_t n, uint32_t lambda,
                 ResultsTablePrinter *table);
    void pool_all(const SolversFabric::AlgTablePtrsVector &fts, size_t threadsAmount);

    const uint32_t N_BEGIN;
    const uint32_t N_END;
    const uint32_t N_STEP;
    const uint32_t LAMBDA_BEGIN;
    const uint32_t LAMBDA_END;
    const uint32_t LAMBDA_STEP;
    const uint32_t TESTS;
    SolversFabric fabric;

public:
    Measurer(const std::shared_ptr<const ParsedParams> &params) : params(params),
        N_BEGIN(params->n[0]), N_END(params->n[1]), N_STEP(params->n[2]),
        LAMBDA_BEGIN(params->lambda[0]), LAMBDA_END(params->lambda[1]), LAMBDA_STEP(params->lambda[2]),
        TESTS(params->tests), fabric(params)
    {}
    void run();

    /*
     * Runs only one (any) algorithm of wantedAlg in params
     * Returns map : n -> amount of evaluations, for the first lambda
    */
    std::shared_ptr<ResultsTable> runOneAlg();
    std::mutex fs_lock;
};

#endif // MEASURER_H
