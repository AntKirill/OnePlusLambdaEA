#include "oneMaxSolver.h"

static constexpr uint INF = std::numeric_limits<int>::max();

uint NextIndexGetter::get(uint ind, double log1prob)
{
    // formula: ind + 1 + log_{log1prob}(r), where log1prob = 1 - p, r = random01()

    double r = engine.random01();
    uint k = ind + 1;
    double p = log(r);
    double l = p / log1prob;
    if (std::isnan(l) || std::isinf(l))
        return INF;
    return k + static_cast<uint>(floor(l));
}

bool OneMaxSolver::mutation(const AbstractOffspring &curParrent, NextIndexGetter &getter,
                            AbstractOffspring_patch &patch, growing_vector<uint> &tmp,
                            const probability_t &p, uint &curChildrenFit, double log1prob)
{
    uint ind = getter.get(-1, log1prob);
    if (ind >= curParrent.bits.size())
    {
        ind = static_cast<uint>(
            floor(getter.engine.random01() * static_cast<double>(curParrent.bits.size())));
        if (ind == curParrent.bits.size())
        {
            --ind;
        }
    }

    while (ind < curParrent.bits.size())
    {
        if (curParrent.bits.test(ind))
            --curChildrenFit;
        else
            ++curChildrenFit;
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
