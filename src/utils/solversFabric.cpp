#include "solversFabric.h"
#include "staticParamsSolver.h"
#include "configParser.h"
#include "adjustingParamsSolver.h"
#include <string>
#include "logger.h"

SolversFabric::SolversFabric(const std::shared_ptr<const ParsedParams> &params) : params(params)
{}

SolversFabric::SolverPtr SolversFabric::init_static() const
{
    return std::make_shared<StaticParamsSolver>(std::vector<double>());
}

SolversFabric::SolverPtr SolversFabric::init_adj2() const
{
    std::vector<double> tmp = {2., 0.5};
    return std::make_shared<AdjustingParamsSolver<2>>(tmp);
}

SolversFabric::SolverPtr SolversFabric::init_adj3() const
{
    return std::make_shared<AdjustingParamsSolver<3>>(params->selfAdjParams);
}

SolversFabric::AlgTablePtrsVector SolversFabric::getAlgos() const
{
    SolversFabric::AlgTablePtrsVector ans;
    for (const auto &s : params->wantedAlgos)
    {
        if (validAlgos.count(s.first))
        {
            SolversFabric::TablePtr tablePtr
                = std::make_shared<ResultsTablePrinter>(s.second, *params.get());
            callback_t f = validAlgos.at(s.first);
            SolversFabric::SolverPtr solverPtr = (this->*f)();
            std::pair<SolversFabric::SolverPtr, SolversFabric::TablePtr> p
                = {std::move(solverPtr), std::move(tablePtr)};
            ans.emplace_back(std::move(p));
            LOG("enabled algorithm:", s.first, "for file", s.second);
        }
    }
    return ans;
}
