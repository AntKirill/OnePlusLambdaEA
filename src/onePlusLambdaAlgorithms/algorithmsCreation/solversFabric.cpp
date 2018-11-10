#include "solversFabric.h"
#include "adjustingParamsSolver.h"
#include "configParser.h"
#include "logger.h"
#include "staticParamsSolver.h"
#include "theoryStaticParams.h"

SolversFabric::SolversFabric(const std::shared_ptr<const ParsedParams> &params) : params(params) {}

SolversFabric::SolverPtr SolversFabric::init_static(uint32_t lambda, uint32_t n) const
{
    return std::make_shared<StaticParamsSolver>(std::vector<double>(), lambda, n,
                                                AlgorithmsTags::StaticParamsTag);
}

SolversFabric::SolverPtr SolversFabric::init_adj2(uint32_t lambda, uint32_t n) const
{
    std::vector<double> tmp = {2., 0.5};
    return std::make_shared<AdjustingParamsSolver<2>>(
        tmp, lambda, n, AlgorithmsTags::AdjustingParamsTwoSubpopulationsTag);
}

SolversFabric::SolverPtr SolversFabric::init_adj3(uint32_t lambda, uint32_t n) const
{
    return std::make_shared<AdjustingParamsSolver<3>>(
        params->selfAdjParams, lambda, n, AlgorithmsTags::AdjustingParamsThreeSubpopulationsTag);
}

SolversFabric::SolverPtr SolversFabric::init_thStatic(uint32_t lambda, uint32_t n) const
{
    return std::make_shared<TheoryStaticParams>(std::vector<double>(), lambda, n,
                                                AlgorithmsTags::TheoryStaticParamsTag);
}

SolversFabric::SolverPtr SolversFabric::create_solver(AlgorithmsTags tag, uint32_t lambda,
                                                      uint32_t n) const
{
    callback_t f = validAlgos.at(tag);
    return (this->*f)(lambda, n);
}

SolversFabric::AlgTablePtrsVector SolversFabric::getAlgos() const
{
    SolversFabric::AlgTablePtrsVector ans;
    for (const auto &s : params->wantedAlgos)
    {
        if (validAlgos.count(s.first))
        {
            SolversFabric::TablePtr tablePtr =
                std::make_shared<ResultsTablePrinter>(s.second, *params.get());
            std::pair<AlgorithmsTags, SolversFabric::TablePtr> p = {s.first, std::move(tablePtr)};
            ans.emplace_back(std::move(p));
            LOG("enabled algorithm:", algorithm_tag_to_str(s.first), "for file", s.second);
        }
    }
    return ans;
}
