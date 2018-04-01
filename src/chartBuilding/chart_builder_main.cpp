#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_set>
#include <string>
#include <vector>

void printUsage(const std::string &s = "")
{
    if (s != "")
    {
        std::cout << "Failer: " << s << std::endl;
    }
    std::cout << "Usage:\n";
    std::cout << "List all files that you want to parse\n";
}

struct Table
{
    std::string name;
    std::map<uint, std::map<uint, std::pair<uint, double>>> data;
    QColor color;
    Table(const std::string &name, const QColor &color) : name(name), color(color) {}
};

std::vector<QColor> colors = {QColor(255, 165, 0), QColor(21, 171, 0), QColor(255, 0, 255), Qt::blue, Qt::green, Qt::red};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc == 1)
    {
        printUsage();
        return 0;
    }
    std::unordered_set<std::string> files;
    for (int i = 1; i < argc; ++i)
    {
        files.emplace(argv[i]);
    }
    using uint = uint32_t;
    std::vector<Table> results;

    uint minN = static_cast<uint>(INT_MAX);
    uint maxN = 0;
    uint minAmount = static_cast<uint>(INT_MAX);
    uint maxAmount = 0;
    std::ifstream fin;
    srand(time(nullptr));
    uint colorCnt = 0;
    for (const auto &name : files)
    {
        QColor color ;
        if (colorCnt >= colors.size())
            color = QColor(rand() % 245 + 10, rand() % 245 + 10, rand() % 245 + 10);
        else color = colors[colorCnt++];
        Table t(name, color);
        fin.open(name);
        if (!fin.good())
        {
            std::string s("no such file ");
            s += name;
            printUsage(s);
            return 0;
        }
        std::string str;
        std::getline(fin, str);
        uint lambda, n, amount;
        double deviation;
        while (fin >> lambda >> n >> amount >> deviation)
        {
            t.data[lambda][n] = std::make_pair(amount, deviation);
        }
        for (const auto &mpLambda : t.data)
        {
            minN = std::min(mpLambda.second.begin()->first, minN);
            maxN = std::max((--mpLambda.second.end())->first, maxN);
            for (const auto &mpN : mpLambda.second)
            {
                minAmount = std::min(mpN.second.first, minAmount);
                maxAmount = std::max(mpN.second.first, maxAmount);
            }
        }
        results.emplace_back(std::move(t));
        fin.close();
    }

    MainWindow w({minN, maxN + 100}, {minAmount, maxAmount + 100});
    for (auto it1 = results[0].data.begin(); it1 != results[0].data.end(); ++it1)
    {
        uint curLambda = it1->first;
        for (auto it2 = results.begin(); it2 != results.end(); ++it2)
        {
            const std::map<uint, std::pair<uint, double>> &nToAmountDev = it2->data[curLambda];
            std::vector<std::pair<uint, uint>> v;
            for (const auto &n : nToAmountDev)
            {
                v.push_back(std::make_pair(n.first, n.second.first));
            }
            w.addNewGraph(v, it2->color, it2->name.c_str());
        }
        w.saveGraphPng(curLambda);
        w.removeAllGraphs();
    }
    return 0;
}
