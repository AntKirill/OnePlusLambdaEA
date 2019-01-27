#include "staticParamsSolver.h"
#include "logger.h"

uint64_t StaticParamsSolver::solve(AbstractOffspring &x, std::shared_ptr<Reporter> reporter_ptr)
{

#ifdef ENABLE_INTERNAL_INFO
    setReporter(reporter_ptr);
    Reporter &reporter = *reporter_ptr.get();
    std::string easy_mutation_filename = "easy_mutations";
    reporter.create_log_file_simple_mutation(easy_mutation_filename);
#endif

    NextIndexGetter getter;
    uint n = static_cast<uint>(x.bits.size());
    probability_t p(n);
    uint ans = 0;
    AbstractOffspring_patch patch(x.fit, p);
    growing_vector<uint> tmp(10);
    double log1prob = log(static_cast<double>(1) - p);

    std::string ns;
    while (x.fit != n)
    {
        for (uint i = 0; i < lambda; ++i)
        {
            uint condidateFit = x.fit;
            mutation(x, getter, patch, tmp, p, condidateFit, log1prob, 0, ns);
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
