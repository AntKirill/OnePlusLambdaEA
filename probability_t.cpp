#include "probability_t.h"

#include <limits>
#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());

bool probability_t::operator()() const
{
    std::uniform_int_distribution<> dis(1, n);
    uint32_t k = dis(gen);
    return (k <= m);
}

probability_t &probability_t::operator/=(uint32_t k)
{
    if (m % k == 0)
    {
        m /= k;
        return *this;
    }
    uint64_t bign = static_cast<uint64_t>(n) * static_cast<uint64_t>(k);
    if (bign < static_cast<uint64_t>(std::numeric_limits<int>::max()))
        n = static_cast<uint32_t>(bign);
    return *this;
}

probability_t &probability_t::operator*=(uint32_t k)
{
    if (n % k == 0)
    {
        n /= k;
        return *this;
    }
    uint64_t bigm = static_cast<uint64_t>(m) * static_cast<uint64_t>(k);
    if (bigm < static_cast<uint64_t>(std::numeric_limits<int>::max()))
        m = static_cast<uint32_t>(bigm);
    return *this;
}

probability_t &probability_t::operator=(const probability_t &rhs)
{
    m = rhs.m;
    n = rhs.n;
    return *this;
}

bool probability_t::operator<(const probability_t &rhs) const
{
    using ul = uint64_t;
    ul m1n2 = static_cast<ul>(m) * static_cast<ul>(rhs.n);
    ul m2n1 = static_cast<ul>(rhs.m) * static_cast<ul>(n);
    return m1n2 < m2n1;
}
