#include <iostream>
#include "resultsTableScanner.h"
#include "metrics.h"
#include <unordered_map>

void printUsage()
{
    std::cout << "First file - base results, second file - delta results";
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
            auto ans = utils::metrics1(parsed[0], parsed[1], mpLambda.first);
            std::cout << "For lambda = " << it->first << " difference is: " << ans << std::endl;
        }
    }
    return 0;
}
