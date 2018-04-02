#include "configParser.h"
#include "measurer.h"
#include <iostream>

int main(int argc, char *argv[])
{
    ParsedParams params;
    if (argc == 1)
    {
        params.n[0] = 100;
        params.n[1] = 1000;
        params.n[2] = 100;
        params.lambda[0] = 3;
        params.lambda[1] = 10;
        params.lambda[2] = 1;
        params.tests = 10;
        params.selfAdjParams = {2., 1., 0.5};
    }
    else
    {
        ConfigParser parser(argv[1]);
        params = parser.parse();
    }
    Measurer m(params);
    m.run();

    return 0;
}
