#ifndef FASTEALGS_H
#define FASTEALGS_H

#include <stdint.h>
#include <boost/dynamic_bitset.hpp>

using uint = uint32_t;

uint oneMaxFitness(const boost::dynamic_bitset<> &bits);

namespace staticMutationProbability
{
    namespace fast
    {
        uint oneMax(const boost::dynamic_bitset<> &bits, uint lambda);
    }
}

namespace adjustingMutationProbabilityWithTwoOffsprings
{
    namespace fast
    {
        uint oneMax(const boost::dynamic_bitset<> &bits, uint lambda);
    }
}

#endif // FASTEALGS_H
