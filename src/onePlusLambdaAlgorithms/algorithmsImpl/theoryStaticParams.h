#ifndef THEORYSTATICPARAMS_H
#define THEORYSTATICPARAMS_H
#include "onePlusLambdaSolver.h"

struct TheoryStaticParams : public OnePlusLambdaSolver
{
    using OnePlusLambdaSolver::OnePlusLambdaSolver;
    uint64_t solve(AbstractOffspring &, std::shared_ptr<Reporter>) override;
};

#endif // THEORYSTATICPARAMS_H
