#include "resultsTableScanner.h"
#include <fstream>

const std::vector<QColor> ResultsTableScanner::colors =
{QColor(255, 165, 0), QColor(21, 171, 0), QColor(255, 0, 255), Qt::blue, Qt::green, Qt::red};

std::vector<ResultsTableScanner> utils::parseResultsFiles(const std::vector<std::string> &files)
{
    std::vector<ResultsTableScanner> ans;
    std::ifstream fin;
    uint colorCnt = 0;
    for (const auto &name : files)
    {
        QColor color;
        if (colorCnt >= ResultsTableScanner::colors.size())
            color = QColor(rand() % 245 + 10, rand() % 245 + 10, rand() % 245 + 10);
        else color = ResultsTableScanner::colors[colorCnt++];
        ResultsTableScanner t(name, color);
        fin.open(name);
        if (!fin.good())
        {
            std::string s("no such file ");
            s += name;
            throw std::runtime_error(s);
        }
        std::string str;
        std::getline(fin, str);
        uint lambda, n, amount;
        double deviation;
        while (fin >> lambda >> n >> amount >> deviation)
        {
            t.data[lambda][n] = std::make_pair(amount, deviation);
        }
        ans.emplace_back(std::move(t));
        fin.close();
    }
    return ans;
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
