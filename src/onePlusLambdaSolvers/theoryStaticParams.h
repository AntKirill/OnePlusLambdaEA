#ifndef THEORYSTATICPARAMS_H
#define THEORYSTATICPARAMS_H
#include "onePlusLambdaSolver.h"

struct TheoryStaticParams : public OnePlusLambdaSolver
{
    using OnePlusLambdaSolver::OnePlusLambdaSolver;
    uint32_t solve(AbstractOffspring &, uint32_t) override;
};

#endif // THEORYSTATICPARAMS_H
