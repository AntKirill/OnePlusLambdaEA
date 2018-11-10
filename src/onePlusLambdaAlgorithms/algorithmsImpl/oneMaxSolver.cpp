#include "oneMaxSolver.h"

uint NextIndexGetter::get(uint ind, double log1prob)
{
    double r = engine.random01();
    uint k = ind + 1;
    double p = log(r);
    return k + static_cast<uint>(floor(p / log1prob));
}

bool OneMaxSolver::mutation(const AbstractOffspring &curParrent, NextIndexGetter &getter,
                            AbstractOffspring_patch &patch, growing_vector<uint> &tmp,
                            const probability_t &p, uint &curChildrenFit, double log1prob)
{
    uint ind;
    do
    {
        ind = getter.get(-1, log1prob);
    } while (ind >= curParrent.bits.size());

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
