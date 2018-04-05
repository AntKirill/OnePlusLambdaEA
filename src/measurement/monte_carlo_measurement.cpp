#include "configParser.h"
#include "measurer.h"
#include <iostream>
#include <stdlib.h>
#include "logger.h"

int main()
{
    ParsedParams params;
    params.n[0] = 10000;
    params.n[1] = 100000;
    params.n[2] = 10000;
    params.lambda[0] = 1600;
    params.lambda[1] = 1600;
    params.lambda[2] = 1;
    params.tests = 10;
    params.wantedAlgos = {{"A3", "resultsSelfAdjustingThree.txt"}};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis1(1.1, 2.1);
    std::uniform_real_distribution<double> dis2(0.2, 0.8);

    const int N = 50;
    for (int i = 1; i <= N; ++i)
    {
        LOG("Test number:", i, "out of:", N);
        params.selfAdjParams = {dis1(gen), 1., dis2(gen)};
        Measurer m(std::make_shared<const ParsedParams>(params));
        m.run();
        {
            int ret;
            ret = system("addParam");
            if (ret != 0)
            {
                LOG(ret);
                std::cerr << "Failed" << std::endl;
            }
        }
    }
    return 0;
}
