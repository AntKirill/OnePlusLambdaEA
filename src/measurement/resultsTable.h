#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include "configParser.h"

struct ResultsTable
{
    ResultsTable(const std::string &fileName, uint32_t offset, const ParsedParams &params);

    void add(uint lambda, uint n, uint evoluations, double deviation);

    void printResults();

    ~ResultsTable() { fout.close(); }
private:

    void print(uint32_t lambda, uint32_t n, uint32_t ans, double deviation);

    void print();

    uint toPrintNextLambda;
    uint toPrintNextN;

    std::string fileName;
    std::ofstream fout;
    const uint32_t offset;
    std::mutex data_mtx;
    /* results[lambda][n] = <evaluations, deviation> */
    std::unordered_map <uint, std::unordered_map<uint, std::pair<uint, double>>> results;
    const uint32_t N_BEGIN;
    const uint32_t N_END;
    const uint32_t N_STEP;
    const uint32_t LAMBDA_BEGIN;
    const uint32_t LAMBDA_END;
    const uint32_t LAMBDA_STEP;
};
