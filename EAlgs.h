#ifndef EALGS_H
#define EALGS_H

#include <functional>
#include <vector>
#include <stddef.h>
#include <random>

void generate(std::vector<bool>&);

namespace staticMutationProbability
{
    uint32_t oneMax(std::vector<bool> x0, uint32_t lambda);
}

namespace adjustingMutationProbabilityWithTwoOffsprings
{
    uint32_t oneMax(std::vector<bool> x0, uint32_t lambda);
}


#endif // EALGS_H
