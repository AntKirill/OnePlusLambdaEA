#include "configParser.h"
#include "logger.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

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

void ConfigParser::parseAlgos(const std::string &)
{
    std::string s;
    std::getline(fin, s);
    boost::trim(s);
    std::vector<std::string> tokens;
    boost::split(tokens, s, boost::is_any_of(" "), boost::token_compress_on);
    params.wantedAlgos[tokens[0]] = tokens[1];
}

ConfigParser::ConfigParser(const std::string &fileName)
{
    fin.open(fileName);
}

std::shared_ptr<const ParsedParams> ConfigParser::parse()
{
    if (!fin.good()) return std::make_shared<const ParsedParams>(params);
    std::string s;
    while (fin >> s)
    {
        if (validHeaders.count(s))
        {
            const call_back_t f = validHeaders.at(s);
            (this->*f)(s);
        }
        else std::getline(fin, s);
    }
    std::sort(params.selfAdjParams.begin(), params.selfAdjParams.end(), [](double a, double b) { return b < a; });
    return std::make_shared<const ParsedParams>(params);
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

