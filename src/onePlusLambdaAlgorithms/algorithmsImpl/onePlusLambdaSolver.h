#ifndef ONEPLUSLAMBDASOLVER_H
#define ONEPLUSLAMBDASOLVER_H

#include "algorithmTags.h"
#include "offspring.h"
#include <boost/dynamic_bitset.hpp>
#include "internalReporter.h"

struct OnePlusLambdaSolver
{
    OnePlusLambdaSolver(const std::vector<double> &params, uint32_t lambda, uint32_t n,
                        AlgorithmsTags tag)
        : params(params), lambda(lambda), offspring_length(n), algorithmsTag(tag)
    {
    }
    virtual uint32_t solve(AbstractOffspring &offspring, std::shared_ptr<Reporter> reporter) = 0;
    virtual ~OnePlusLambdaSolver() {}
    const std::vector<double> *get_params_ptr() const {
        if (params.size() == 0) return nullptr;
        return &params;
    }

protected:
    const std::vector<double> params;
    uint32_t lambda;
    uint32_t offspring_length;
    AlgorithmsTags algorithmsTag;
    std::shared_ptr<Reporter> my_reporter_ptr = nullptr;

    void setReporter(std::shared_ptr<Reporter> reporter_ptr)
    {
        this->my_reporter_ptr = reporter_ptr;
    }


};

#endif // ONEPLUSLAMBDASOLVER_H
