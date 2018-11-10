#ifndef ONEMAXSOLVER_H
#define ONEMAXSOLVER_H

#include "oneMaxOffspring.h"
#include "onePlusLambdaSolver.h"

struct NextIndexGetter
{
    NextIndexGetter() = default;
    uint get(uint ind, double log1prob);
    utils::fast_random engine;
};

struct OneMaxSolver : public OnePlusLambdaSolver
{
    using OnePlusLambdaSolver::OnePlusLambdaSolver;

protected:
    // TODO : reduce amount of arguments
    virtual bool mutation(const AbstractOffspring &curParrent, NextIndexGetter &getter,
                          AbstractOffspring_patch &patch, growing_vector<uint> &tmp,
                          const probability_t &p, uint &curChildrenFit, double log1prob);
};

#endif // ONEMAXSOLVER_H
