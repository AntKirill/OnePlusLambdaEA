#include "nextIndexGetter.h"

uint NextIndexGetter::get(uint ind, double log1prob)
{
    double r = engine.random01();
    uint k = ind + 1;
    double p = log(r);
    return k + static_cast<uint>(floor(p / log1prob));
}
