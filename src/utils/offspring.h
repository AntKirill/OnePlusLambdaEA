#ifndef OFFSPRING_H
#define OFFSPRING_H

#include <boost/dynamic_bitset.hpp>
#include "probability_t.h"
#include "growing_vector.h"

struct AbstractOffspring
{
    boost::dynamic_bitset<> bits; //gens
    uint32_t fit; //fitness of cur offspring
    probability_t p; //probability with which this offspring have been produced

    struct AbstractOffspring_patch
    {
        uint32_t fit;
        probability_t p;
        growing_vector<uint32_t> toChange;
        AbstractOffspring_patch(uint32_t fit, const probability_t &p) : fit(fit), p(p), toChange(10) {}
    };

    AbstractOffspring &operator=(const AbstractOffspring_patch &rhs_patch)
    {
        for (size_t i = 0; i < rhs_patch.toChange.buzy; ++i) bits.flip(rhs_patch.toChange[i]);
        fit = rhs_patch.fit;
        p = rhs_patch.p;
        return *this;
    }

    virtual uint32_t fitness(const boost::dynamic_bitset<> &) const = 0;

    ~AbstractOffspring() = default;


protected:

    AbstractOffspring(const boost::dynamic_bitset<> &bits, uint32_t fit, const probability_t &p) :
        bits(bits), fit(fit), p(p) { }


};

using AbstractOffspring_patch = AbstractOffspring::AbstractOffspring_patch;

#endif // OFFSPRING_H
