#ifndef SEARCH_COMMON_H
#define SEARCH_COMMON_H

#include <memory>
#include "resultsTableScanner.h"
#include "configParser.h"
#include <vector>
#include <fstream>

namespace search
{
    bool addNewResult();
    bool updateRes(std::shared_ptr<ResultsTable>, std::shared_ptr<ResultsTable>, bool &);
    bool isBig(double);
    void handleParams(double &, double &);
    void dump(std::ofstream &, const std::vector<double> &);

    struct Common
    {
        Common();
        virtual ~Common() = default;
        void search();
    protected:
        using Pdd = std::pair<double, double>;
        virtual void do_search() = 0;
        virtual void config();
        std::string bestResFile = "bestResults.txt";
        ParsedParams params;
        std::shared_ptr<ResultsTableScanner> bestResPtr;
        bool measureAndUpdate(int testNumber);
    private:
        void init_bestRes();
        void finish();
        bool update_bestRes(std::shared_ptr<ResultsTable>, std::shared_ptr<ResultsTable> newResPtr);
        bool wasUpdate = false;
        std::vector<double> bestParams;
        int goodTest;
    };
}

#endif // SEARCH_COMMON_H
