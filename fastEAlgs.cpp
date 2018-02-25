#include "fastEAlgs.h"
#include "probability_t.h"
#include "math.h"
#include "random"
#include <iostream>
#include <limits>
#include <array>

static std::random_device rd;
static std::mt19937 gen(rd());

const size_t N = 1000010;

std::array<uint, N> ids; // id of bits to change

struct offspring_patch_t
{
    uint toChangeAmount;
    uint fit;
    probability_t p;
    offspring_patch_t(uint fit, const probability_t &p) : toChangeAmount(0), fit(fit), p(p) {}
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
        for (uint i = 0; i < rhs_patch.toChangeAmount; ++i)
            bits[ids[i]] = !bits[ids[i]];
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

static inline uint get_next_ind(int ind, const probability_t &p)
{
    static const double N = std::numeric_limits<double>::max();
    std::uniform_real_distribution<double> dis(0, N);
    double r = dis(gen) / N;
    uint k = static_cast<uint>(ind + 1);
    return k + static_cast<uint>(floor(logn(oneMinus(p), r)));
}

static bool mutation(uint ind, const offspring_t &x, uint &cur_fit, const probability_t &p, offspring_patch_t &patch)
{
    if (ind >= x.bits.size())
    {
        if (cur_fit >= patch.fit)
        {
            patch.toChangeAmount = 0;
            patch.fit = cur_fit;
            patch.p = p;
            return true;
        }
        return false;
    }
    if (x.bits[ind]) --cur_fit;
    else ++cur_fit;
    if (mutation(get_next_ind(ind, p), x, cur_fit, p, patch))
    {
        ids[patch.toChangeAmount++] = ind;
        return true;
    }
    return false;
}

uint staticMutationProbability::fast::oneMax(std::vector<bool> bits, uint lambda)
{
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
            mutation(get_next_ind(-1, p), x, condidateFit, p, patch);
        }
        x = patch;
        ans += lambda;
    }
    return ans;
}

static const probability_t half(2);
static const probability_t p2(4);

uint adjustingMutationProbabilityWithTwoOffsprings::fast::
oneMax(std::vector<bool> bits, uint lambda)
{
    uint n = static_cast<uint>(bits.size());
    const probability_t p1(n / 2);
    probability_t p(n);
    auto x = offspring_t(bits, p);
    uint ans = 0;
    uint halflambda = lambda / 2;
    probability_t pBest(n);

    auto doMutation = [&](bool & wasUpdate, uint from, uint to, offspring_patch_t &patch)
    {
        uint delta = n + 1;
        for (uint i = from; i < to; ++i)
        {
            uint condidateFit = x.fit;
            bool updated = mutation(get_next_ind(-1, p), x, condidateFit, p, patch);
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
        p *= 2;
        doMutation(wasUpdate, 0, halflambda, patch);
        p /= 4;
        doMutation(wasUpdate, halflambda, lambda, patch);
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
