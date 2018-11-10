#ifndef SOLVERSFABRIC_H
#define SOLVERSFABRIC_H

#include "algorithmTags.h"
#include "onePlusLambdaSolver.h"
#include "resultsTablePrinter.h"
#include <string>
#include <unordered_map>

struct ParsedParams;

struct SolversFabric
{
    using AlgTablePtrsVector =
        std::vector<std::pair<AlgorithmsTags, std::shared_ptr<ResultsTablePrinter>>>;
    SolversFabric(const std::shared_ptr<const ParsedParams> &);

private:
    const std::shared_ptr<const ParsedParams> params;
    using callback_t = std::shared_ptr<OnePlusLambdaSolver> (SolversFabric::*)(uint32_t,
                                                                               uint32_t) const;
    using SolverPtr = std::shared_ptr<OnePlusLambdaSolver>;
    using TablePtr = std::shared_ptr<ResultsTablePrinter>;
    SolverPtr init_static(uint32_t lambda, uint32_t n) const;
    SolverPtr init_adj2(uint32_t lambda, uint32_t n) const;
    SolverPtr init_adj3(uint32_t lambda, uint32_t n) const;
    SolverPtr init_thStatic(uint32_t lambda, uint32_t n) const;

public:
    const std::unordered_map<AlgorithmsTags, callback_t> validAlgos = {
        {AlgorithmsTags::StaticParamsTag, &SolversFabric::init_static},
        {AlgorithmsTags::AdjustingParamsTwoSubpopulationsTag, &SolversFabric::init_adj2},
        {AlgorithmsTags::AdjustingParamsThreeSubpopulationsTag, &SolversFabric::init_adj3},
        {AlgorithmsTags::TheoryStaticParamsTag, &SolversFabric::init_thStatic}};
    AlgTablePtrsVector getAlgos() const;
    SolversFabric::SolverPtr create_solver(AlgorithmsTags tag, uint32_t lambda, uint32_t n) const;
};

#endif // SOLVERSFABRIC_H
