#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <ostream>

struct ParsedParams
{
    std::vector<double> selfAdjParams = {2., 1., 0.5};
    std::array<uint32_t, 3> lambda = {{3, 10, 1}}; //0: begin, 1: end, 2: step
    std::array<uint32_t, 3> n = {{100, 1000, 100}}; //0: begin, 1: end, 2: step
    uint32_t tests = 10;
    ParsedParams() : selfAdjParams(3, 0) {}
};

std::ostream &operator<<(std::ostream &, const ParsedParams &);

class ConfigParser
{
    std::ifstream fin;
    using call_back_t = void (ConfigParser::*)(const std::string &);
    const std::unordered_map<std::string, call_back_t> validHeaders =
    {
        {"selfAdjParams:", &ConfigParser::parseSelfAdj},
        {"N_BEGIN:", &ConfigParser::parseN},
        {"N_END:", &ConfigParser::parseN},
        {"N_STEP:", &ConfigParser::parseN},
        {"LAMBDA_BEGIN:", &ConfigParser::parseLambda},
        {"LAMBDA_END:", &ConfigParser::parseLambda},
        {"LAMBDA_STEP:", &ConfigParser::parseLambda},
        {"TESTS:", &ConfigParser::parseTests}
    };
    void parseSelfAdj(const std::string&);
    void parseN(const std::string &);
    void parseLambda(const std::string &);
    void parseTests(const std::string &);
    ParsedParams params;
public:
    ConfigParser(const std::string &);
    ParsedParams parse();
    ~ConfigParser();
};

#endif // CONFIGPARSER_H
