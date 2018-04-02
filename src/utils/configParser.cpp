#include "configParser.h"
#include "logger.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

void ConfigParser::parseSelfAdj(const std::string &)
{
    double d;
    for (int i = 0; i < 3; i++)
    {
        fin >> d;
        params.selfAdjParams[i] = d;
    }
}

void ConfigParser::parseN(const std::string &name)
{
    uint32_t d;
    fin >> d;
    if (name == "N_BEGIN:") params.n[0] = d;
    if (name == "N_END:") params.n[1] = d;
    if (name == "N_STEP:") params.n[2] = d;
}

void ConfigParser::parseLambda(const std::string &name)
{
    uint32_t d;
    fin >> d;
    if (name == "LAMBDA_BEGIN:") params.lambda[0] = d;
    if (name == "LAMBDA_END:") params.lambda[1] = d;
    if (name == "LAMBDA_STEP:") params.lambda[2] = d;
}

void ConfigParser::parseTests(const std::string &)
{
    uint32_t d;
    fin >> d;
    params.tests = d;
}

ConfigParser::ConfigParser(const std::string &fileName)
{
    fin.open(fileName);
}

ParsedParams ConfigParser::parse()
{
    if (!fin.good()) return params;
    std::string s;
    while (fin >> s)
        if (validHeaders.count(s))
        {
            const call_back_t f = validHeaders.at(s);
            (this->*f)(s);
        }
    std::sort(params.selfAdjParams.begin(), params.selfAdjParams.end(), [](double a, double b) { return b < a; });
    return params;
}

ConfigParser::~ConfigParser()
{
    fin.close();
}

std::ostream &operator<<(std::ostream &of, const ParsedParams &p)
{
    of << "LAMBDA: ";
    for (auto i : p.lambda) of << i << " ";
    of << std::endl;
    of << "N: ";
    for (auto i : p.n) of << i << " ";
    of << std::endl;
    of << "TESTS: ";
    of << p.tests << std::endl;
    of << "selfAdjParams: ";
    for (auto i : p.selfAdjParams) of << i << " ";
    of << std::endl;
    return of;
}

