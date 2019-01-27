#include "adjustingParamsSolver.h"

template<uint32_t subpopulations>
uint64_t AdjustingParamsSolver<subpopulations>::solve(AbstractOffspring &x,
                                                      std::shared_ptr<Reporter> reporter_ptr)
{
    if (subpopulations <= 1)
        throw std::runtime_error("invalid amount of populations");
    if (params.size() < subpopulations)
        throw std::runtime_error("too few params");

#ifdef ENABLE_INTERNAL_INFO
    setReporter(reporter_ptr);
    Reporter &reporter = *reporter_ptr.get();
    std::string probability_file_name = "probability";
    reporter.create_log_file("iteration_number", "probability", probability_file_name);
    std::string max_fitness_file_name = "max_fitness";
    reporter.create_log_file("iteration_number", "max_fitness", max_fitness_file_name);
    std::string easy_mutation_filename = "easy_mutations";
    reporter.create_log_file_simple_mutation(easy_mutation_filename);
#endif
    probability_t half(2);
    probability_t p2(4);

    uint n = static_cast<uint>(x.bits.size());
    probability_t p(x.p);
    //    const probability_t p1(n / 2);
    const probability_t p1(static_cast<double>(1. / (static_cast<double>(n) * static_cast<double>(n))));
    uint64_t ans = 0;
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

    uint iteration_number = 0;
    auto doMutation = [&](bool &wasUpdate, uint &delta, uint from, uint to)
    {
        double log1prob = log(1 - p);
        for (uint i = from; i < to; ++i)
        {
            uint condidateFit = x.fit;
            bool updated =
                mutation(x, getter, patch, tmp, p, condidateFit, log1prob, iteration_number,
#ifdef ENABLE_INTERNAL_INFO
                    easy_mutation_filename);
#else
                         "");
#endif
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
#ifdef ENABLE_INTERNAL_INFO
        reporter.report_data(iteration_number, p.to_double(), probability_file_name);
#endif
        bool wasUpdate = false;
        uint delta = n + 1;
        for (uint i = 0; i < segmlambda.size() - 1; ++i)
        {
            p *= params[i];
            doMutation(wasUpdate, delta, segmlambda[i], segmlambda[i + 1]);
            p /= params[i];
        }
#ifdef ENABLE_INTERNAL_INFO
        reporter.report_data(iteration_number, patch.fit, max_fitness_file_name);
#endif
        if (wasUpdate)
        {
            x = patch;
            pBest = x.p;
        }
        if (half())
            p = pBest;
        else if (half())
            p *= params[0];
        else
            p *= params.back();
        p = std::min(std::max(p1, p), p2);
        ans += static_cast<uint64_t>(lambda);
        patch.toChange.reset();
        ++iteration_number;
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

template
struct AdjustingParamsSolver<2>;
template
struct AdjustingParamsSolver<3>;
