#ifndef RESULTSTABLESCANNER_H
#define RESULTSTABLESCANNER_H

#include "resultsTable.h"
#include <vector>

struct ResultsTableScanner;

namespace utils
{
    std::vector<ResultsTable> parseResultsFiles(const std::vector<std::string> &);
}

struct ResultsTableScanner : public ResultsTable
{
    ResultsTableScanner(const std::string &fileName) : ResultsTable{fileName} {}
    void parse();
    void open(const std::string &fileName);
    std::pair<uint, uint> getMinMaxN() const;
    std::pair<uint, uint> getMinMaxAmount() const;
};

#endif // RESULTSTABLESCANNER_H
