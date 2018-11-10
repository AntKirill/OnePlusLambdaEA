#ifndef LEADINGONESSOLVER_H
#define LEADINGONESSOLVER_H

#include "nextIndexGetter.h"
#include "offspring.h"
#include "onePlusLambdaSolver.h"
#include "probability_t.h"
#include <boost/dynamic_bitset.hpp>

struct LeadingOnesOffspring : public AbstractOffspring
{
    LeadingOnesOffspring(const boost::dynamic_bitset<> &bits, const probability_t &p)
        : AbstractOffspring(bits, 0, p)
    {
        AbstractOffspring::fit = fitness(bits);
    }

    uint32_t fitness(const boost::dynamic_bitset<> &x) const
    {
        for (size_t i = 0; i != x.size(); ++i)
        {
            if (!x.test(i))
                return i;
        }
        return x.size();
    }
};

struct LeadingOnesSolver : public OnePlusLambdaSolver
{
    using OnePlusLambdaSolver::OnePlusLambdaSolver;

private:
    void continue_mutation(uint ind, NextIndexGetter &getter, const AbstractOffspring &curParrent,
                           growing_vector<uint> &tmp, double log1prob) const
    {
        while (ind < curParrent.bits.size())
        {
            tmp.my_push_back(ind);
            ind = getter.get(ind, log1prob);
        }
    }

    bool change_patch(uint curChildrenFit, AbstractOffspring_patch &patch,
                      growing_vector<uint> &tmp, const probability_t &p)
    {
        if (curChildrenFit >= patch.fit)
        {
            patch.fit = curChildrenFit;
            patch.toChange.swap(tmp);
            patch.p = p;
            return true;
        }
        return false;
    }

protected:
    // TODO : reduce amount of arguments
    virtual bool mutation(const AbstractOffspring &curParrent, NextIndexGetter &getter,
                          AbstractOffspring_patch &patch, growing_vector<uint> &tmp,
                          const probability_t &p, uint &curChildrenFit, double log1prob)
    {
        uint ind;
        do
        {
            ind = getter.get(-1, log1prob);
        } while (ind >= curParrent.bits.size());

        bool updated = false;

        uint firstzero_ind = curParrent.fit;
        if (ind < firstzero_ind)
        {
            // became worse
            updated = false;
            goto quit;
        }
        else if (ind > firstzero_ind)
        {
            // not became better, but new offspring with the same fit
            continue_mutation(ind, getter, curParrent, tmp, log1prob);
            change_patch(curChildrenFit, patch, tmp, p);
            updated = true;
            goto quit;
        }
        else
        {
            // fitness increased
            ++curChildrenFit;
            tmp.my_push_back(ind);
            for (ind = getter.get(ind, log1prob); ind < curParrent.bits.size();
                 ind = getter.get(ind, log1prob))
            {
                for (size_t i = firstzero_ind; i != ind; ++i)
                {
                    if (!curParrent.bits.test(i))
                    {
                        continue_mutation(ind, getter, curParrent, tmp, log1prob);
                        change_patch(curChildrenFit, patch, tmp, p);
                        updated = true;
                        goto quit;
                    }
                    ++curChildrenFit;
                }
                if (curParrent.bits.test(ind))
                {
                    continue_mutation(ind, getter, curParrent, tmp, log1prob);
                    change_patch(curChildrenFit, patch, tmp, p);
                    updated = true;
                    goto quit;
                }
                else
                {
                    ++curChildrenFit;
                }
            }
            continue_mutation(ind, getter, curParrent, tmp, log1prob);
            change_patch(curChildrenFit, patch, tmp, p);
            updated = true;
            goto quit;
        }
    quit:
        tmp.reset();
        return updated;
    }
};

#endif // LEADINGONESSOLVER_H
