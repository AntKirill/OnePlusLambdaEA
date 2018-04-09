#include "resultsTableScanner.h"
#include <fstream>
#include <limits>

std::vector<ResultsTable> utils::parseResultsFiles(const std::vector<std::string> &files)
{
    std::vector<ResultsTable> ans;
    for (const auto &name : files)
    {
        ResultsTableScanner t(name);
        t.parse();
        ans.emplace_back(std::move(t));
    }
    return ans;
}

void ResultsTableScanner::parse()
{
    std::ifstream fin;
    fin.open(fileName);
    if (!fin.good())
    {
        std::string s("no such file ");
        s += fileName;
        throw std::runtime_error(s);
    }
    std::string str;
    std::getline(fin, str);
    uint lambda, n, amount;
    double deviation;
    while (fin >> lambda >> n >> amount >> deviation)
    {
        data[lambda][n] = std::make_pair(amount, deviation);
    }
    fin.close();
}

void ResultsTableScanner::open(const std::string &fileName)
{
    this->fileName = fileName;
    this->data.clear();
}

std::pair<uint, uint> ResultsTableScanner::getMinMaxN() const
{
    uint minN = std::numeric_limits<uint>::max();
    uint maxN = 0;
    for (const auto &mpLambda : data)
    {
        for (const auto &mpN : mpLambda.second)
        {
            minN = std::min(mpN.first, minN);
            maxN = std::max(mpN.first, maxN);
        }
    }
    return {minN, maxN};
}

std::pair<uint, uint> ResultsTableScanner::getMinMaxAmount() const
{
    uint minAmount = std::numeric_limits<uint>::max();
    uint maxAmount = 0;
    for (const auto &mpLambda : data)
    {
        for (const auto &mpN : mpLambda.second)
        {
            minAmount = std::min(mpN.second.first, minAmount);
            maxAmount = std::max(mpN.second.first, maxAmount);
        }
    }
    return {minAmount, maxAmount};
}
