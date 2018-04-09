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

    MainWindow w;
    const auto &dataRes0 = results[0].getData();
    for (auto it1 = dataRes0.begin(); it1 != dataRes0.end(); ++it1)
    {
        uint curLambda = it1->first;
        w.resetColors();
        for (auto it2 = results.begin(); it2 != results.end(); ++it2)
        {
            auto dataIt2 = it2->getData();
            const auto &nToAmountDev = dataIt2[curLambda];
            std::vector<double> x;
            std::vector<double> y;
            std::vector<double> d;
            for (const auto &n : nToAmountDev)
            {
                x.push_back(static_cast<double>(n.first));
                y.push_back(static_cast<double>(n.second.first));
                d.push_back(static_cast<double>(n.second.second));
            }
            std::sort(x.begin(), x.end());
            std::sort(y.begin(), y.end());
            std::sort(d.begin(), d.end());
            w.addNewGraph(x, y, d, it2->getFileName().c_str());
        }
        w.saveGraphPng(curLambda);
        w.removeAllGraphs();
    }
    return 0;
}
