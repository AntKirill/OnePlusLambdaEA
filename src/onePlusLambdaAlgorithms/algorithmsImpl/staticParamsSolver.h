#ifndef STATICPARAMSSOLVER_H
#define STATICPARAMSSOLVER_H

#include "oneMaxSolver.h"

struct StaticParamsSolver : public OneMaxSolver
{
    using OneMaxSolver::OneMaxSolver;
    uint32_t solve(AbstractOffspring &, std::shared_ptr<Reporter>) override;
};

#endif // STATICPARAMSSOLVER_H
