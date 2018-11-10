#include "search_common.h"
#include "logger.h"
#include "measurer.h"
#include "metrics.h"
#include "resultsTableScanner.h"
#include <iostream>

bool search::addNewResult()
{
    int ret;
    ret = system("addParam");
    if (ret != 0)
    {
        LOG(ret);
        std::cerr << "Failed" << std::endl;
        return false;
    }
    return true;
}

bool search::updateRes(std::shared_ptr<ResultsTable> best, std::shared_ptr<ResultsTable> cur,
                       bool &wasUpdate)
{
    int64_t n = utils::metrics1(*best.get(), *cur.get(), best->firstLambda());
    if (n > 0)
    {
        best = cur;
        wasUpdate = true;
        return true;
    }
    return false;
}

bool search::isBig(double d)
{
    if (d > 1.)
    {
        if (d > 1.55)
            return true;
        return false;
    }
    else
    {
        if (d > 0.55)
            return true;
        return false;
    }
}

void search::handleParams(double &d1, double &d2)
{
    if (isBig(d1) && isBig(d2))
        d2 = 1 - d2;
    if (!isBig(d1) && !isBig(d2))
        d2 = 1 - d2;
}

void search::dump(std::ofstream &off, const std::vector<double> &v)
{
    off << "selfAdjParams: ";
    for (auto i : v)
        off << i << " ";
    off << std::endl;
}

search::Common::Common()
{
    params.n[0] = 10000;
    params.n[1] = 100000;
    params.n[2] = 10000;
    params.lambda[0] = 3200;
    params.lambda[1] = 3200;
    params.lambda[2] = 1;
    params.tests = 10;
    params.wantedAlgos = {
        {AlgorithmsTags::AdjustingParamsThreeSubpopulationsTag, "resultsSelfAdjustingThree.txt"}};
}

void search::Common::search()
{
    config();
    init_bestRes();
    do_search();
    finish();
}

void search::Common::config() {}

void search::Common::init_bestRes()
{
    bestResPtr = std::make_shared<ResultsTableScanner>(bestResFile);
    bestResPtr->parse();
}

bool search::Common::measureAndUpdate(int testNumber)
{
    LOG("run test number:", testNumber);
    std::ofstream fout("dumpAdj.txt");
    search::dump(fout, params.selfAdjParams);
    fout.close();

    Measurer m(std::make_shared<const ParsedParams>(params));
    std::shared_ptr<ResultsTable> res = m.runOneAlg();
    if (update_bestRes(bestResPtr, res))
    {
        LOG("New best params found: ", params.selfAdjParams[0], params.selfAdjParams[1],
            params.selfAdjParams[2]);
        bestParams = params.selfAdjParams;
        goodTest = testNumber;
        std::ofstream fout;
        fout.open("bestParams.txt");
        search::dump(fout, params.selfAdjParams);
        fout.close();
        std::string begComand = "cat resultsSelfAdj* > ";
        int ok = system((begComand + bestResFile).c_str());
        if (ok != 0)
            LOG("Faild to update results");
        bestResPtr->open(bestResFile);
        bestResPtr->parse();
    }
    if (!search::addNewResult())
        return false;
    SEP();
    return true;
}

void search::Common::finish()
{
    if (wasUpdate)
    {
        LOG("Best params:", bestParams[0], bestParams[1], bestParams[2], "on test number",
            goodTest);
    }
    else
    {
        LOG("Best params stay the same");
    }
}

bool search::Common::update_bestRes(std::shared_ptr<ResultsTable> best,
                                    std::shared_ptr<ResultsTable> newResPtr)
{
    int64_t n = utils::metrics1(*best.get(), *newResPtr.get(), newResPtr->firstLambda());
    if (n > 0)
    {
        best = newResPtr;
        wasUpdate = true;
        return true;
    }
    return false;
}
