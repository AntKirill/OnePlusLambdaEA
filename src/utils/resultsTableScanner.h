#ifndef RESULTSTABLESCANNER_H
#define RESULTSTABLESCANNER_H

#include "resultsTable.h"
#include <vector>
#include <QColor>

struct ResultsTableScanner;

namespace utils
{
    std::vector<ResultsTableScanner> parseResultsFiles(const std::vector<std::string> &);
}

class ResultsTableScanner : public ResultsTable
{
    static const std::vector<QColor> colors;
    QColor color;
    ResultsTableScanner(const std::string &fileName, const QColor &color) :
        ResultsTable{fileName}, color(color) {}
public:
    friend std::vector<ResultsTableScanner> utils::parseResultsFiles(const std::vector<std::string> &);
    std::pair<uint, uint> getMinMaxN() const;
    std::pair<uint, uint> getMinMaxAmount() const;
    QColor getColor() { return color; }
};

#endif // RESULTSTABLESCANNER_H
