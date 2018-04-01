#ifndef GROWING_VECTOR_H
#define GROWING_VECTOR_H

#include <vector>

template <typename T>
struct growing_vector
{
    growing_vector(size_t beg_size) : wrapped(beg_size) {}
    void reset() { buzy = 0; }
    void my_push_back(T x)
    {
        if (buzy < wrapped.size()) wrapped[buzy++] = x;
        else { wrapped.push_back(x); ++buzy; }
    }
    const T &operator[](size_t i) const { return wrapped[i]; }
    T &operator [](size_t i) { return wrapped[i]; }
    void swap(growing_vector &rhs)
    {
        wrapped.swap(rhs.wrapped);
        std::swap(buzy, rhs.buzy);
    }
    std::vector<T> wrapped;
    size_t buzy = 0;
};


#endif // GROWING_VECTOR_H
