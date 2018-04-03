#ifndef RESULTSTABLE_H
#define RESULTSTABLE_H

#include <unordered_map>
#include <string>

struct ResultsTable
{

    ResultsTable(const std::string &fileName) : fileName(fileName) {}
    virtual ~ResultsTable() {}

    const auto &getData() const { return data; }

    const std::string &getFileName() { return fileName; }

protected:
    /* results[lambda][n] = <evaluations, deviation> */
    std::unordered_map <uint, std::unordered_map<uint, std::pair<uint, double>>> data;
    std::string fileName;


};

#endif // RESULTSTABLE_H
