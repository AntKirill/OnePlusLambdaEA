#ifndef OFFSPRINGGENERATOR_H
#define OFFSPRINGGENERATOR_H

#include "configParser.h"
#include "offspring.h"
#include <boost/dynamic_bitset.hpp>
#include <random>
#include "LeadingOnesSolver.h"
#include "oneMaxSolver.h"

namespace offspring_generator
{
std::shared_ptr<AbstractOffspring> generate(boost::dynamic_bitset<> &x, std::mt19937 &engine,
                                            std::shared_ptr<const ParsedParams> params_ptr)
{
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = static_cast<bool>(dis(engine));
    if (params_ptr->wantedProblem == ParsedParams::LeadingOnes) {
        return std::make_shared<LeadingOnesOffspring>(LeadingOnesOffspring(x, probability_t(1. / static_cast<double>(x.size()))));
    } else if (params_ptr->wantedProblem == ParsedParams::OneMax) {
        return std::make_shared<LeadingOnesOffspring>(OneMaxOffspring(x, probability_t(1. / static_cast<double>(x.size()))));
    }
    return nullptr;
}
}

#endif // OFFSPRINGGENERATOR_H
