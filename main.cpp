#include <QApplication>
#include "mainwindow.h"
#include <functional>
#include <vector>
#include <stddef.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <EAlgs.h>
#include "fastEAlgs.h"
#include "logger.h"

inline double getStandartDeviation(uint32_t ans, const std::vector<double> &t)
{
    double t_av = static_cast<double>(ans);
    double s = 0;
    double n = static_cast<double>(t.size());
    for (auto ti : t)
    {
        s += (ti - t_av) * (ti - t_av);
    }
    s /= n;
    return sqrt(s);
}

struct resultsTable
{
    resultsTable(const std::string &fileName, uint32_t offset) : fout(fileName), offset(offset)
    {
        fout << std::setw(offset) << "lambda" << std::setw(offset) << "n" << std::setw(
                 offset) << "function evaluations" << std::setw(offset) << "standard deviation" << std::endl;
        fout << std::endl;
    }

    void add(uint32_t lambda, uint32_t n, uint32_t ans, double deviation)
    {
        fout << std::setw(offset) << lambda << std::setw(offset) << n << std::setw(
                 offset) << ans << std::setw(offset) << deviation << std::endl;
    }

    void add()
    {
        fout << std::endl;
    }

    ~resultsTable()
    {
        fout.close();
    }

private:
    std::ofstream fout;
    const uint32_t offset;
};

using uint = uint32_t;

const uint N_BEGIN = 10000;
const uint N_END = 100000;
const uint N_STEP = 10000;

const uint LAMBDA_BEGIN = 200;
const uint LAMBDA_END = 200;
const uint LAMBDA_STEP = 1;

const uint TESTS = 100;

template <typename F>
uint32_t average(F f, uint32_t n, uint32_t lambda, resultsTable &table)
{
    uint32_t ans = 0;
    std::vector<double> t;
    for (uint32_t test = 0; test < TESTS; ++test)
    {
        std::vector<bool> offs(n, false);
        generate(offs);
        uint32_t ti = f(offs, lambda);
        ans += ti;
        t.push_back(static_cast<double>(ti));

        if (test % 25 == 0)
            LOG("Passed test № ", test, "for lambda = ", lambda, " for n = ", n);
    }
    ans /= TESTS;
    table.add(lambda, n, ans, getStandartDeviation(ans, t));
    return ans;
}

template <typename F>
void measure(F f, resultsTable &table, MainWindow &w, QColor graphColor,
             const char *nameOfGraphLine, uint32_t lambda)
{
    LOG("\n");
    LOG("Started line: ", nameOfGraphLine);
    LOG("\n");

    std::vector<std::pair<uint32_t, uint32_t>> prob;
    for (uint32_t n = N_BEGIN; n <= N_END; n += N_STEP)
    {
        uint32_t ans = average(f, n, lambda, table);
        prob.push_back(std::make_pair(n, ans));
        LOG("Done for lambda = ", lambda, ", n = ", n, ", for line: ", nameOfGraphLine);
    }
    table.add();
    LOG("Done for lambda = ", lambda, ", for line: ", nameOfGraphLine);
    w.addNewGraph(prob, graphColor, nameOfGraphLine);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    resultsTable table("resultsStatic.txt", 25);
    resultsTable tableAdjusting("resultsAdjusting.txt", 25);

    for (uint32_t lambda = LAMBDA_BEGIN; lambda <= LAMBDA_END; lambda += LAMBDA_STEP)
    {
        measure(staticMutationProbability::fast::oneMax, table, w, Qt::blue, "Static Mutation Probability",
                lambda);
        measure(adjustingMutationProbabilityWithTwoOffsprings::fast::oneMax, tableAdjusting, w, Qt::red,
                "Adjusting Mutation Probability", lambda);
        w.saveGraphPng(lambda);
        w.removeAllGraphs();
    }
    return 0;
}
