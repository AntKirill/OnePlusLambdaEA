#ifndef NEXTINDEXGETTER_H
#define NEXTINDEXGETTER_H

#include "fast_random.h"

struct NextIndexGetter
{
    NextIndexGetter() = default;
    uint get(uint ind, double log1prob);

private:
    utils::fast_random engine;
};

#endif // NEXTINDEXGETTER_H
