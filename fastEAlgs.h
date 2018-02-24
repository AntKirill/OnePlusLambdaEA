#ifndef FASTEALGS_H
#define FASTEALGS_H

#include <vector>
#include <stdint.h>

using uint = uint32_t;

uint oneMaxFitness(const std::vector<bool> &bits);

namespace staticMutationProbability
{
    namespace fast
    {
        uint oneMax(std::vector<bool> bits, uint lambda);
    }
}

namespace adjustingMutationProbabilityWithTwoOffsprings
{
    namespace fast
    {
        uint oneMax(std::vector<bool> bits, uint lambda);
    }
}

#endif // FASTEALGS_H
