#include "probability_t.h"
#include <limits>
#include <random>
#include <stdexcept>

probability_t::probability_t(const probability_t &rhs)
{
    *this = rhs;
}

bool probability_t::operator()()
{
    if (p < 0) throw std::runtime_error("negative probability");
    double k = engine.random01();
    return (k <= p);
}

probability_t &probability_t::operator/=(double k)
{
    p /= k;
    return *this;
}

probability_t &probability_t::operator*=(double k)
{
    p *= k;
    return *this;
}

probability_t &probability_t::operator=(const probability_t &rhs)
{
    p = rhs.p;
    engine = rhs.engine;
    return *this;
}

bool probability_t::operator<(const probability_t &rhs) const
{
    return p < rhs.p;
}

double operator-(double a, const probability_t &p)
{
    return a - p.p;
}
