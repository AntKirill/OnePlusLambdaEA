#include "oneMaxSolver.h"

static constexpr double EPS = 0.00000000001;

static constexpr uint INF = std::numeric_limits<int>::max();

inline uint NextIndexGetter::get(uint ind, double log1prob)
{
    // formula: ind + 1 + log_{log1prob}(r), where log1prob = 1 - p, r = random01()

    if (fabs(log1prob) < EPS)
        return INF;
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
                            const probability_t &p, uint &curChildrenFit, double log1prob,
                            uint iteration_number, const std::string &report_file_name)
{
    uint ind = getter.get(-1, log1prob);
    const uint curParrentSize = static_cast<uint>(curParrent.bits.size());
    bool was_easy_mutation = false;
    if (ind >= curParrentSize)
    {
        ind = static_cast<uint>(
            floor(getter.engine.random01() * static_cast<double>(curParrentSize)));
        if (ind == curParrentSize) --ind;
        was_easy_mutation = true;
    }

    while (ind < curParrentSize)
    {
        if (curParrent.bits.test(ind))
            --curChildrenFit;
        else
            ++curChildrenFit;
        tmp.my_push_back(ind);
        if (was_easy_mutation)
            break;
        ind = getter.get(ind, log1prob);
    }

#ifdef ENABLE_INTERNAL_INFO
    if (was_easy_mutation && !first_usual_mutation_happen) {
        my_reporter_ptr->report_simple_mutation(iteration_number, report_file_name);
    }
#endif

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
