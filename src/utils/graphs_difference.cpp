#include <iostream>
#include "resultsTableScanner.h"
#include <unordered_map>

void printUsage()
{
    std::cout << "First file - base results, second file - delta results";
}

using MPN = const std::unordered_map<uint, std::pair<uint, double>> &;

int64_t norm(MPN mpn1, MPN mpn2)
{
    using intt = int64_t;
    int64_t ans = 0;
    for (const auto &itn1 : mpn1)
    {
        auto it2 = mpn2.find(itn1.first);
        ans += static_cast<intt>(itn1.second.first) - static_cast<intt>(it2->second.first);
    }
    return ans;
}

int main(int argc, char * argv[])
{
    if (argc != 3)
    {
        printUsage();
        return 0;
    }
    std::string res1(argv[1]);
    std::string res2(argv[2]);
    auto parsed = utils::parseResultsFiles({res1, res2});
    const auto &data1 = parsed[0].getData();
    const auto &data2 = parsed[1].getData();
    for (const auto &mpLambda : data1)
    {
        auto it = data2.find(mpLambda.first);
        if (it != data2.end())
        {
            auto ans = norm(mpLambda.second, it->second);
            std::cout << "For lambda = " << it->first << " difference is: " << ans << std::endl;
        }
    }
    return 0;
}
