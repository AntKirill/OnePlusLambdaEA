#ifndef ADJUSTINGPARAMSSOLVER_H
#define ADJUSTINGPARAMSSOLVER_H

#include "oneMaxSolver.h"

template <uint32_t subpopulations>
struct AdjustingParamsSolver : public OneMaxSolver
{
    using OneMaxSolver::OneMaxSolver;
    uint32_t solve(AbstractOffspring &, uint32_t) override;
};

#endif // ADJUSTINGPARAMSSOLVER_H
