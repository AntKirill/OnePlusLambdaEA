#include "EAlgs.h"
#include "probability_t.h"

using offspring_t = std::vector<bool>;
using fitness_t = std::function<uint32_t(const std::vector<bool>&)>;
using uint = uint32_t;

std::pair<offspring_t, int> argMax(fitness_t cnt, const std::vector<offspring_t> &x,
                                   uint32_t &lastMaxCnt)
{
    int curBestId = 0;
    lastMaxCnt = 0;
    for (int i = 0; i < static_cast<int>(x.size()); ++i)
    {
        uint32_t c = cnt(x[i]);
        if (lastMaxCnt < c)
        {
            curBestId = i;
            lastMaxCnt = c;
        }
    }
    return {x[curBestId], curBestId};
}

offspring_t flip(offspring_t x, probability_t &p)
{
    for (size_t i = 0; i < x.size(); ++i)
    {
        if (p()) x[i] = !x[i];
    }
    return x;
}

uint cnt(const std::vector<bool> &bits)
{
    uint ans = 0;
    for (auto i : bits) if (i) ++ans;
    return ans;
}

void generate(std::vector<bool> &x)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (size_t i = 0; i < x.size(); ++i)
        x[i] = static_cast<bool>(dis(gen));
}

uint staticMutationProbability::oneMax(offspring_t x0, uint32_t lambda)
{
    uint32_t n = static_cast<uint32_t>(x0.size());
    std::vector<std::vector<bool>> x(lambda, std::vector<bool>(n, 0));
    for (auto &xi : x) generate(xi);
    x[0] = x0;
    uint32_t lastMaxCnt = 0;
    probability_t p(n);
    offspring_t parent = x[0];
    uint32_t cntParent = cnt(parent);
    uint ans = 0;
    while (cntParent != n)
    {
        for (uint32_t i = 0; i < lambda; ++i)
        {
            x[i] = flip(parent, p);
        }
        offspring_t condidate = argMax(cnt, x, lastMaxCnt).first;
        if (cntParent <= lastMaxCnt)
        {
            parent.swap(condidate);
            cntParent = lastMaxCnt;
        }
        ans += lambda;
    }
    return ans;
}

const probability_t half(2);
const probability_t p2(4);

uint adjustingMutationProbabilityWithTwoOffsprings::oneMax(offspring_t x0, uint32_t lambda)
{
    const uint32_t n = static_cast<uint32_t>(x0.size());
    std::vector<std::vector<bool>> x(lambda, std::vector<bool>(n, 0));
    for (auto &xi : x) generate(xi);
    x[0] = x0;
    uint32_t lastMaxCnt = 0;
    probability_t p(n);
    offspring_t parent = x[0];
    uint32_t cntParent = cnt(parent);
    std::vector<probability_t> probabilitySave(lambda);
    const probability_t p1(n / 2);
    uint32_t halfLambda = (lambda - 1) / 2;
    uint ans = 0;
    while (cntParent != n)
    {
        p /= 2;
        bool was = false;
        for (uint32_t i = 0; i < lambda; ++i)
        {
            x[i] = flip(parent, p);
            probabilitySave[i] = p;
            if (!was && (i >= halfLambda))
            {
                p *= 4;
                was = true;
            }
        }
        int condidateId = argMax(cnt, x, lastMaxCnt).second;
        if (cntParent <= lastMaxCnt)
        {
            parent.swap(x[condidateId]);
            cntParent = lastMaxCnt;
        }

        if (half())
        {
            p = probabilitySave[condidateId];
        }
        else
        {
            if (half())
            {
                p = probabilitySave[0];
            }
            else
            {
                p = probabilitySave.back();
            }
        }

        p = std::min(std::max(p1, p), p2);
        ans += lambda;
    }
    return ans;
}

