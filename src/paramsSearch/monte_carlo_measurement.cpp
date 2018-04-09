#include "search_common.h"
#include <random>
#include "logger.h"


struct Searcher : public search::Common
{

    Searcher(Pdd a, Pdd b) :
        search::Common{}, gen(rd()), dis1(a.first, a.second), dis2(b.first, b.second) {}

private:
    const int N = 10;

    void do_search() override
    {
        for (int i = 1; i <= N; ++i)
        {
            LOG("Test number:", i, "out of:", N);
            double d1 = dis1(gen);
            double d2 = dis2(gen);
            search::handleParams(d1, d2);
            params.selfAdjParams = {d1, 1., d2};
            measureAndUpdate(i);
        }
    }

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis1;
    std::uniform_real_distribution<double> dis2;
};

int main()
{
    Searcher({1.3, 1.4}, {0.6, 0.7}).search();
    return 0;
}



