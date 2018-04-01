#pragma once

#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>

extern const uint N_BEGIN;
extern const uint N_END;
extern const uint N_STEP;

extern const uint LAMBDA_BEGIN;
extern const uint LAMBDA_END;
extern const uint LAMBDA_STEP;

extern const uint TESTS;


struct ResultsTable
{
    ResultsTable(const std::string &fileName, uint32_t offset);

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
};
