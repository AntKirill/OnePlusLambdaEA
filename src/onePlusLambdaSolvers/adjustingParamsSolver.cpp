#include "adjustingParamsSolver.h"
#include "oneMaxSolver.h"
#include <stdexcept>
#include <array>
#include "logger.h"

template<uint32_t subpopulations>
uint32_t AdjustingParamsSolver<subpopulations>::solve(AbstractOffspring &x, uint32_t lambda)
{
    if (subpopulations <= 1) throw std::runtime_error("invalid amount of populations");
    if (params.size() < subpopulations) throw std::runtime_error("too few params");

    probability_t half(2);
    probability_t p2(4);

    uint n = static_cast<uint>(x.bits.size());
    probability_t p(x.p);
    const probability_t p1(n / 2);
    uint ans = 0;
    std::array<uint, subpopulations + 1> segmlambda;
    segmlambda[0] = 0;
    for (uint i = 1; i < segmlambda.size(); ++i)
    {
        segmlambda[i] = i * lambda / subpopulations;
    }
    probability_t pBest(n);

    NextIndexGetter getter;

    AbstractOffspring_patch patch(x.fit, x.p);
    growing_vector<uint> tmp(10);

    auto doMutation = [&](bool & wasUpdate, uint & delta, uint from, uint to)
    {
        double log1prob = log(1 - p);
        for (uint i = from; i < to; ++i)
        {
            uint condidateFit = x.fit;
            bool updated = mutation(x, getter, patch, tmp, p, condidateFit, log1prob);
            if (!updated && (delta > x.fit - condidateFit))
            {
                delta = x.fit - condidateFit;
                pBest = p;
            }
            wasUpdate |= updated;
        }
    };

    while (x.fit != n)
    {
        bool wasUpdate = false;
        uint delta = n + 1;
        for (uint i = 0; i < segmlambda.size() - 1; ++i)
        {
            p *= params[i];
            doMutation(wasUpdate, delta, segmlambda[i], segmlambda[i + 1]);
            p /= params[i];
        }
        if (wasUpdate)
        {
            x = patch;
            pBest = x.p;
        }
        if (half()) p = pBest;
        else if (half()) p *= params[0];
        else p *= params.back();
        p = std::min(std::max(p1, p), p2);
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

template struct AdjustingParamsSolver<2>;
template struct AdjustingParamsSolver<3>;
