#ifndef SOLVERSFABRIC_H
#define SOLVERSFABRIC_H

#include <unordered_map>
#include <string>
#include "onePlusLambdaSolver.h"
#include "resultsTablePrinter.h"

struct ParsedParams;

struct SolversFabric
{
    using AlgTablePtrsVector = std::vector<std::pair<std::shared_ptr<OnePlusLambdaSolver>, std::shared_ptr<ResultsTablePrinter>>>;
    SolversFabric(const std::shared_ptr<const ParsedParams> &);
private:
    const std::shared_ptr<const ParsedParams> params;
    using callback_t = std::shared_ptr<OnePlusLambdaSolver> (SolversFabric::*)() const;
    using SolverPtr = std::shared_ptr<OnePlusLambdaSolver>;
    using TablePtr = std::shared_ptr<ResultsTablePrinter>;
    SolverPtr init_static() const;
    SolverPtr init_adj2() const;
    SolverPtr init_adj3() const;
public:
    const std::unordered_map<std::string, callback_t> validAlgos =
    {
        {"St", &SolversFabric::init_static},
        {"A2", &SolversFabric::init_adj2},
        {"A3", &SolversFabric::init_adj3}
    };
    AlgTablePtrsVector getAlgos() const;
};

#endif // SOLVERSFABRIC_H
