#ifndef PROBABILITY_T_H
#define PROBABILITY_T_H

#include <stdint.h>

struct probability_t
{

    probability_t() {}

    probability_t(uint32_t n) : m(1), n(n) {}

    // returns true with this probability
    bool operator()() const;

    probability_t &operator/=(uint32_t k);

    probability_t &operator*=(uint32_t k);

    probability_t &operator=(const probability_t &rhs);

    bool operator<(const probability_t &rhs) const;


private:
    // this probability = m / n;

    friend double oneMinus(const probability_t &p);
    uint32_t m;
    uint32_t n;
};

#endif // PROBABILITY_T_H
