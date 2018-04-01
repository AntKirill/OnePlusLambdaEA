#ifndef ONEPLUSLAMBDASOLVER_H
#define ONEPLUSLAMBDASOLVER_H

#include <boost/dynamic_bitset.hpp>
#include "offspring.h"

struct OnePlusLambdaSolver
{
    OnePlusLambdaSolver(const std::vector<double> &params) : params(params) {}
    virtual uint32_t solve(AbstractOffspring &offspring, uint32_t lambda) = 0;
    virtual ~OnePlusLambdaSolver() {}
protected:
    const std::vector<double> params;
};

#endif // ONEPLUSLAMBDASOLVER_H
