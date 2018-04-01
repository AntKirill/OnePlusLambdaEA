#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string.h>

template<typename TF>
static void write_debug_output(std::ostream &out, TF const &f)
{
    out << f;
}

static int _path_length = strrchr(__FILE__, '/') ? strlen(__FILE__) -
                          strlen(strrchr(__FILE__, '/')) + 1 : 0;

struct tracer
{
    std::ostream &out;
    tracer(std::ostream &out, char const *file, int line, const char *func) : out(out)
    {
        out << file + _path_length << " : " << func << " : " << line << " : ";
    }
    ~tracer()
    {
        out << std::endl;
    }

    template<typename TF, typename ... TR>
    void write(TF const &f, TR const &... rest)
    {
        write_debug_output(out, f);
        out << " ";
        write(rest...);
    }

    template<typename TF>
    void write(TF const &f)
    {
        write_debug_output(out, f);
    }

    void write()
    {
        //handle the empty params case
    }
};

#define LOG(...) tracer( std::cout, __FILE__, __LINE__, __func__ ).write( __VA_ARGS__ )

#else

#define LOG(...)

#endif // DEBUG_H
