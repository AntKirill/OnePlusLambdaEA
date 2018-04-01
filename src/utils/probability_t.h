#ifndef PROBABILITY_T_H
#define PROBABILITY_T_H

#include <stdint.h>
#include <random>
#include "fast_random.h"

struct probability_t
{
    probability_t() = default;
    explicit probability_t(int n) : p(1. / static_cast<double>(n)) {}
    explicit probability_t(uint32_t n) : p(1. / static_cast<double>(n)) {}
    explicit probability_t(double p) : p(p) {}
    explicit probability_t(const probability_t &rhs);
    bool operator ()();
    probability_t &operator/=(double k);
    probability_t &operator*=(double k);
    probability_t &operator=(const probability_t &rhs);
    bool operator<(const probability_t &rhs) const;
    friend double operator-(double, const probability_t &);
private:
    utils::fast_random engine;
    friend double oneMinus(const probability_t &p);
    double p;
};

#endif // PROBABILITY_T_H
