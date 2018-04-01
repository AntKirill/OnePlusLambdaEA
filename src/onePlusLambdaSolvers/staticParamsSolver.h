#ifndef STATICPARAMSSOLVER_H
#define STATICPARAMSSOLVER_H

#include "oneMaxSolver.h"

struct StaticParamsSolver : public OneMaxSolver
{
    using OneMaxSolver::OneMaxSolver;
    uint32_t solve(AbstractOffspring &, uint32_t) override;
};

#endif // STATICPARAMSSOLVER_H
