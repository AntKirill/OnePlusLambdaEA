#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_set>
#include <string>
#include <vector>
#include "resultsTableScanner.h"

void printUsage(const std::string &s = "")
{
    if (s != "")
    {
        std::cout << "Failer: " << s << std::endl;
    }
    std::cout << "Usage:\n";
    std::cout << "List all files that you want to parse\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc == 1)
    {
        printUsage();
        return 0;
    }
    std::vector<std::string> files;
    for (int i = 1; i < argc; ++i)
    {
        files.emplace_back(argv[i]);
    }

    auto results = utils::parseResultsFiles(files);
    std::pair<uint, uint> minMaxN;
    std::pair<uint, uint> minMaxAmount;
    auto update = [](std::pair<uint, uint> oldMinMax,
                     std::pair<uint, uint> newMinMaxN) -> std::pair<uint, uint>
    {
        return {std::min(oldMinMax.first, newMinMaxN.first), std::max(oldMinMax.second, newMinMaxN.second)};
    };
    for (const auto &table : results)
    {
        minMaxN = update(minMaxN, table.getMinMaxN());
        minMaxAmount = update(minMaxAmount, table.getMinMaxAmount());
    }

    MainWindow w({minMaxN.first, minMaxN.second + 100}, {minMaxAmount.first, minMaxAmount.second + 100});
    auto dataRes0 = results[0].getData();
    for (auto it1 = dataRes0.begin(); it1 != dataRes0.end(); ++it1)
    {
        uint curLambda = it1->first;
        for (auto it2 = results.begin(); it2 != results.end(); ++it2)
        {
            auto dataIt2 = it2->getData();
            const auto &nToAmountDev = dataIt2[curLambda];
            std::vector<std::pair<uint, uint>> v;
            for (const auto &n : nToAmountDev)
            {
                v.push_back(std::make_pair(n.first, n.second.first));
            }
            std::sort(v.begin(), v.end(),
            [](const std::pair<uint, uint> &a, const std::pair<uint, uint> &b) { return a.first < b.first; });
            w.addNewGraph(v, it2->getColor(), it2->getFileName().c_str());
        }
        w.saveGraphPng(curLambda);
        w.removeAllGraphs();
    }
    return 0;
}
