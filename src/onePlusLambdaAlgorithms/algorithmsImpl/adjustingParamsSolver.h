#ifndef ADJUSTINGPARAMSSOLVER_H
#define ADJUSTINGPARAMSSOLVER_H

#include "oneMaxSolver.h"
#include "internalReporter.h"

template <uint32_t subpopulations>
struct AdjustingParamsSolver : public OneMaxSolver
{
    using OneMaxSolver::OneMaxSolver;
    uint32_t solve(AbstractOffspring &, std::shared_ptr<Reporter>) override;
};

#endif // ADJUSTINGPARAMSSOLVER_H
