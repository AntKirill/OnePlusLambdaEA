#include "fastEAlgs.h"
#include "probability_t.h"
#include "math.h"
#include <iostream>
#include <cassert>
#include "logger.h"
#include "fast_random.h"
#include <boost/dynamic_bitset.hpp>
#include <vector>

struct offspring_t;

struct NextIndexGetter;

template <typename T>
struct growing_vector
{
    growing_vector(size_t beg_size) : wrapped(beg_size) {}
    void reset() { buzy = 0; }
    void my_push_back(T x)
    {
        if (buzy < wrapped.size()) wrapped[buzy++] = x;
        else { wrapped.push_back(x); ++buzy;}
    }
    const T &operator[](size_t i) const
    {
        return wrapped[i];
    }
    T &operator [](size_t i)
    {
        return wrapped[i];
    }
    void swap(growing_vector &rhs)
    {
        wrapped.swap(rhs.wrapped);
        std::swap(buzy, rhs.buzy);
    }
    std::vector<T> wrapped;
    size_t buzy = 0;
};


struct offspring_patch_t
{
    uint fit;
    probability_t p;
    growing_vector<uint> toChange;
    offspring_patch_t(uint fit, const probability_t &p) : fit(fit), p(p), toChange(10) {}
};

struct offspring_t
{
    boost::dynamic_bitset<> bits;
    uint32_t fit;
    probability_t p;

    offspring_t(const boost::dynamic_bitset<> &bits, const probability_t &p) : bits(bits), p(p)
    {
        fit = oneMaxFitness(bits);
    }

    offspring_t(const boost::dynamic_bitset<> &bits, uint fit, const probability_t &p) : bits(bits),
        fit(fit),
        p(p) {}

    offspring_t &operator=(const offspring_patch_t &rhs_patch)
    {
        for (size_t i = 0; i < rhs_patch.toChange.buzy; ++i) bits.flip(rhs_patch.toChange[i]);
        fit = rhs_patch.fit;
        p = rhs_patch.p;
        return *this;
    }
};

uint oneMaxFitness(const boost::dynamic_bitset<> &bits)
{
    return bits.count();
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
    NextIndexGetter() = default;
    uint get(uint ind, double log1prob)
    {
        double r = engine.random01();
        uint k = ind + 1;
        double p = log(r);
        return k + static_cast<uint>(floor(p / log1prob));
    }
private:
    utils::fast_random engine;
};

bool mutation(const offspring_t &curParrent, NextIndexGetter &getter,
              offspring_patch_t &patch, growing_vector<uint> &tmp, const probability_t &p, uint &curChildrenFit, double log1prob)
{
    uint ind = getter.get(-1, log1prob);

    while (ind < curParrent.bits.size())
    {
        if (curParrent.bits.test(ind)) --curChildrenFit;
        else ++curChildrenFit;
        tmp.my_push_back(ind);
        ind = getter.get(ind, log1prob);
    }

    bool updated = false;
    if (curChildrenFit >= patch.fit)
    {
        patch.fit = curChildrenFit;
        patch.toChange.swap(tmp);
        patch.p = p;
        updated = true;
    }
    tmp.reset();
    return updated;
}

uint staticMutationProbability::fast::oneMax(const boost::dynamic_bitset<> &bits, uint lambda)
{
    NextIndexGetter getter;
    uint n = static_cast<uint>(bits.size());
    probability_t p(n);
    auto x = offspring_t(bits, p);
    uint ans = 0;
    offspring_patch_t patch(x.fit, p);
    growing_vector<uint> tmp(10);
    double log1prob = log(oneMinus(p));

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

uint adjustingMutationProbabilityWithTwoOffsprings::fast::
oneMax(const boost::dynamic_bitset<> &bits, uint lambda)
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

    offspring_patch_t patch(x.fit, x.p);
    growing_vector<uint> tmp(10);

    auto doMutation = [&](bool & wasUpdate, uint & delta, uint from, uint to)
    {
        double log1prob = log(oneMinus(p));
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
        p *= 2;
        doMutation(wasUpdate, delta, 0, halflambda);
        p /= 4;
        doMutation(wasUpdate, delta, halflambda, lambda);
        if (wasUpdate)
        {
            x = patch;
            pBest = x.p;
        }
        if (half()) p = pBest;
        else if (half()) p *= 4;
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
