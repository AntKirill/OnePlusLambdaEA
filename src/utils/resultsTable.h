#ifndef RESULTSTABLE_H
#define RESULTSTABLE_H

#include <unordered_map>
#include <string>

struct ResultsTable
{

    virtual ~ResultsTable() {}

    const auto &getData() const { return data; }

    const std::string &getFileName() const { return fileName; }

    std::unordered_map<uint, uint> getMapFromNToRes(uint lambda) const
    {
        std::unordered_map<uint, uint> ans;
        auto it = data.find(lambda) ;
        if (it != data.end())
        {
            for (const auto &d : it->second)
            {
                ans[d.first] = d.second.first;
            }
        }
        return ans;
    }

    std::unordered_map<uint, uint> getMapFromNToRes() const
    {
        return getMapFromNToRes(data.begin()->first);
    }

    uint firstLambda() const { return data.begin()->first; }


protected:
    ResultsTable(const std::string &fileName) : fileName(fileName) {}
    /* results[lambda][n] = <evaluations, deviation> */
    std::unordered_map<uint, std::unordered_map<uint, std::pair<uint, double>>> data;
    std::string fileName;


};

#endif // RESULTSTABLE_H
