#include "theoryStaticParams.h"


uint32_t TheoryStaticParams::solve(AbstractOffspring &offsprint, uint32_t lambda)
{
    double n = static_cast<double>(offsprint.bits.size());
    double l = static_cast<double>(lambda);
    double rt = l * n * log(log(l)) / (1.369 * log(l)) + n * log(n);
    return static_cast<uint>(rt);
}
