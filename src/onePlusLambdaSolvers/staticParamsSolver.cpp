#include "staticParamsSolver.h"
#include "logger.h"

uint32_t StaticParamsSolver::solve(AbstractOffspring &x, uint32_t lambda)
{
    NextIndexGetter getter;
    uint n = static_cast<uint>(x.bits.size());
    probability_t p(n);
    uint ans = 0;
    AbstractOffspring_patch patch(x.fit, p);
    growing_vector<uint> tmp(10);
    double log1prob = log(static_cast<double>(1) - p);

    while (x.fit != n)
    {
        for (uint i = 0; i < lambda; ++i)
        {
            uint condidateFit = x.fit;
            mutation(x, getter, patch, tmp, p, condidateFit, log1prob);
        }
        x = patch;
        ans += lambda;
        patch.toChange.reset();
    }
    #ifndef NDEBUG
    if (x.bits.count() != x.bits.size())
    {
        LOG("Expected bits amount : ", x.bits.size(), " found : ", x.bits.count());
        assert(false);
    }
    #endif
    return ans;
}
