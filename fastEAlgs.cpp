#include "fastEAlgs.h"
#include "probability_t.h"
#include "math.h"
#include "random"
#include <iostream>
#include <limits>
#include <array>
#include "thread"

static const size_t N = 1000010;

struct offspring_patch_t
{
    std::vector<uint> toChange;
    uint fit;
    probability_t p;
    offspring_patch_t(uint fit, const probability_t &p) : toChange(0), fit(fit), p(p) {}
};

struct offspring_t
{
    std::vector<bool> bits;
    uint32_t fit;
    probability_t p;

    offspring_t(const std::vector<bool> &bits, const probability_t &p) : bits(bits), p(p)
    {
        fit = oneMaxFitness(bits);
    }

    offspring_t(const std::vector<bool> &bits, uint fit, const probability_t &p) : bits(bits), fit(fit),
        p(p) {}

    offspring_t &operator=(const offspring_patch_t &rhs_patch)
    {
        for (auto i : rhs_patch.toChange)
            bits[i] = !bits[i];
        fit = rhs_patch.fit;
        p = rhs_patch.p;
        return *this;
    }
};

uint oneMaxFitness(const std::vector<bool> &bits)
{
    uint ans = 0;
    for (auto i : bits) if (i) ++ans;
    return ans;
}

static inline double logn(double base, double r)
{
    return log(r) / log(base);
}

double oneMinus(const probability_t &p)
{
    return static_cast<double>(p.n - p.m) / static_cast<double>(p.n);
}

struct NextIndexGetter
{
    NextIndexGetter() : gen(rd()) {}
    uint get(int ind, double log1prob)
    {
        std::uniform_real_distribution<double> dis(0, 1);
        double r = dis(gen);
        uint k = static_cast<uint>(ind + 1);
        return k + static_cast<uint>(floor(log(r) / log1prob));
    }
private:
    std::random_device rd;
    std::mt19937 gen;
};

static bool mutation(const offspring_t &curParrent, NextIndexGetter &getter,
                     offspring_patch_t &patch, const probability_t &p, uint &curChildrenFit)
{
    double log1prob = log(oneMinus(p));
    uint ind = getter.get(-1, log1prob);
    std::vector<uint> curToChange;

    while (ind < curParrent.bits.size())
    {
        if (curParrent.bits[ind]) --curChildrenFit;
        else ++curChildrenFit;
        curToChange.push_back(ind);
        ind = getter.get(ind, log1prob);
    }

    bool updated = false;
    if (curChildrenFit >= patch.fit)
    {
        patch.fit = curChildrenFit;
        patch.toChange.swap(curToChange);
        patch.p = p;
        updated = true;
    }
    return updated;
}

uint staticMutationProbability::fast::oneMax(const std::vector<bool> &bits, uint lambda)
{
    NextIndexGetter getter;
    uint n = static_cast<uint>(bits.size());
    probability_t p(n);
    auto x = offspring_t(bits, p);
    uint ans = 0;

    while (x.fit != n)
    {
        offspring_patch_t patch(x.fit, p);
        for (uint i = 0; i < lambda; ++i)
        {
            uint condidateFit = x.fit;
            mutation(x, getter, patch, p, condidateFit);
        }
        x = patch;
        ans += lambda;
    }
    return ans;
}

uint adjustingMutationProbabilityWithTwoOffsprings::fast::
oneMax(const std::vector<bool> &bits, uint lambda)
{
    static const probability_t half(2);
    static const probability_t p2(4);

    uint n = static_cast<uint>(bits.size());
    const probability_t p1(n / 2);
    probability_t p(n);
    auto x = offspring_t(bits, p);
    uint ans = 0;
    uint halflambda = lambda / 2;
    probability_t pBest(n);

    NextIndexGetter getter;
    auto doMutation = [&](bool & wasUpdate, uint & delta, uint from, uint to, offspring_patch_t &patch)
    {
        for (uint i = from; i < to; ++i)
        {
            uint condidateFit = x.fit;
            bool updated = mutation(x, getter, patch, p, condidateFit);
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
        offspring_patch_t patch(x.fit, x.p);
        bool wasUpdate = false;
        uint delta = n + 1;
        p *= 2;
        doMutation(wasUpdate, delta, 0, halflambda, patch);
        p /= 4;
        doMutation(wasUpdate, delta, halflambda, lambda, patch);
        if (wasUpdate)
        {
            x = patch;
            pBest = x.p;
        }
        if (half()) p = pBest;
        else if (half()) p *= 4;
        p = std::min(std::max(p1, p), p2);
        ans += lambda;
    }
    return ans;
}
