#pragma once

#include <mutex>
#include <fstream>
#include "configParser.h"
#include "resultsTable.h"

struct ResultsTablePrinter : ResultsTable
{
    ResultsTablePrinter(const std::string &fileName, uint32_t offset, const ParsedParams &params);

    ResultsTablePrinter(const std::string &fileName,
                        const ParsedParams &params) : ResultsTablePrinter(fileName, 25, params) {}

    void add(uint lambda, uint n, uint evoluations, double deviation);

    void printResults();

    ~ResultsTablePrinter() { fout.close(); }

private:

    void print(uint32_t lambda, uint32_t n, uint32_t ans, double deviation);

    void print();

    uint toPrintNextLambda;
    uint toPrintNextN;

    std::ofstream fout;
    const uint32_t offset;
    std::mutex data_mtx;
    const uint32_t N_BEGIN;
    const uint32_t N_END;
    const uint32_t N_STEP;
    const uint32_t LAMBDA_BEGIN;
    const uint32_t LAMBDA_END;
    const uint32_t LAMBDA_STEP;
};
