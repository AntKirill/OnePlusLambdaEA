#include "resultsTable.h"
#include <iostream>
#include <iomanip>
#include "logger.h"

ResultsTable::ResultsTable(const std::string &fileName,
                           uint32_t offset) : toPrintNextLambda(LAMBDA_BEGIN),
    toPrintNextN(N_BEGIN), fileName(fileName), fout(fileName), offset(offset)
{
    fout << std::setw(offset) << "lambda" << std::setw(offset) << "n" << std::setw(
             offset) << "function evaluations" << std::setw(offset) << "standard deviation" << std::endl;
    fout << std::endl;
}

void ResultsTable::add(uint lambda, uint n, uint evoluations, double deviation)
{
    data_mtx.lock();
    results[lambda][n] = std::make_pair(evoluations, deviation);
    LOG("Done for lambda = ", lambda, ", n = ", n, ", for line: ", fileName);
    data_mtx.unlock();
}

void ResultsTable::printResults()
{
    data_mtx.lock();
    for (uint &i = toPrintNextLambda; i <= LAMBDA_END; i += LAMBDA_STEP)
    {
        if (toPrintNextN > N_END) toPrintNextN = N_BEGIN;
        if (!results.count(i)) break;
        bool ok = false;
        for (uint &j = toPrintNextN; j <= N_END; j += N_STEP)
        {
            ok = false;
            if (!results[i].count(j)) break;
            ok = true;
            print(i, j, results[i][j].first, results[i][j].second);
        }
        if (!ok) break;
        print();
    }
    data_mtx.unlock();
}

void ResultsTable::print(uint32_t lambda, uint32_t n, uint32_t ans, double deviation)
{
    fout << std::setw(offset) << lambda << std::setw(offset) << n << std::setw(
             offset) << ans << std::setw(offset) << deviation << std::endl;
}

void ResultsTable::print()
{
    fout << std::endl;
}
