#ifndef ONEMAXOFFSPRING_H
#define ONEMAXOFFSPRING_H

#include "offspring.h"
#include <boost/dynamic_bitset.hpp>

struct OneMaxOffspring : public AbstractOffspring
{
    OneMaxOffspring(const boost::dynamic_bitset<> &bits, const probability_t &p)
        : AbstractOffspring(bits, fitness(bits), p)
    {
    }
    uint32_t fitness(const boost::dynamic_bitset<> &bits) const override { return bits.count(); }
};

#endif // ONEMAXOFFSPRING_H
