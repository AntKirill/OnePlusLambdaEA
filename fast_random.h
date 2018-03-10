#ifndef FAST_RANDOM_H
#define FAST_RANDOM_H

#include <random>
#include <limits>

namespace utils
{

    class fast_random
    {

        using ldbl = long double;

        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<uint64_t> dis;
        uint64_t state[2];
        const ldbl Max;

        static inline uint64_t rotl(const uint64_t x, int k)
        {
            return (x << k) | (x >> (64 - k));
        }

        uint64_t xorshift128Plus()
        {
            const uint64_t s0 = state[0];
            uint64_t s1 = state[1];
            const uint64_t result = s0 + s1;

            s1 ^= s0;
            state[0] = rotl(s0, 55) ^ s1 ^ (s1 << 14);
            state[1] = rotl(s1, 36);

            return result;
        }

    public:
        fast_random() : gen(rd()),
            dis(std::numeric_limits<int64_t>::max(), std::numeric_limits<uint64_t>::max()),
            Max(static_cast<ldbl>(std::numeric_limits<uint64_t>::max()))
        {
            for (int i = 0; i < 2; ++i) state[i] = dis(gen);
        }

        double random01() { return static_cast<ldbl>(xorshift128Plus()) / Max; }

    };
}

#endif // FAST_RANDOM_H
