#ifndef ONEMAXSOLVER_H
#define ONEMAXSOLVER_H

#include "onePlusLambdaSolver.h"
#include "fast_random.h"
#include "probability_t.h"
#include <boost/dynamic_bitset.hpp>
#include "offspring.h"

struct OneMaxOffspring : public AbstractOffspring
{
    OneMaxOffspring(const boost::dynamic_bitset<> &bits, const probability_t &p) :
        AbstractOffspring(bits, fitness(bits), p) {}
    uint32_t fitness(const boost::dynamic_bitset<> &bits) const override
    {
        return bits.count();
    }
};

struct NextIndexGetter
{
    NextIndexGetter() = default;
    uint get(uint ind, double log1prob);

private:
    utils::fast_random engine;
};

struct OneMaxSolver : public OnePlusLambdaSolver
{
    using OnePlusLambdaSolver::OnePlusLambdaSolver;
protected:
    // TODO : reduce amount of arguments
    virtual bool mutation(const AbstractOffspring &curParrent, NextIndexGetter &getter,
                          AbstractOffspring_patch &patch, growing_vector<uint> &tmp,
                          const probability_t &p, uint &curChildrenFit,
                          double log1prob);
};

#endif // ONEMAXSOLVER_H
