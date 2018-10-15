#include "search_common.h"

struct Searcher : public search::Common
{
    using Common::Common;
private:

    bool isEq(double a, double b, double eps)
    {
        double d = a - b;
        return ((d > -eps) && (d < eps));
    }

    bool wantToSkip(double a, double epsa, double b, double epsb)
    {
        return (isEq(a, 1.3, epsa) && isEq(b, 0.65, epsb));
    }

    void do_search() override
    {
        Pdd a3paramFirst = {1.4, 1.4};
        Pdd a3paramSecond = {0.65, 0.65};
        double stepFirst = 0.1;
        double stepSecond = 0.05;
        a3paramFirst.second += stepFirst;
        a3paramSecond.second += stepSecond;
        double curFirst = a3paramFirst.first;
        double curSecond = a3paramSecond.first;
        double epsFirst = stepFirst / 2;
        double epsSecond = stepSecond / 2;
        int cnt = 0;
        while (true)
        {
            params.selfAdjParams = {curFirst, 1., curSecond};
            if (!wantToSkip(curFirst, epsFirst, curSecond, epsSecond))
                measureAndUpdate(++cnt);
            curFirst += stepFirst;
            if (isEq(curFirst, a3paramFirst.second, epsFirst))
            {
                curFirst = a3paramFirst.first;
                curSecond += stepSecond;
            }
            if (isEq(curSecond, a3paramSecond.second, epsSecond))
            {
                break;
            }
        }
    }
};

int main()
{
    Searcher().search();
    return 0;
}
