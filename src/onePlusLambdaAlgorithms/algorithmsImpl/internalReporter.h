#ifndef INTERNALREPORTER_H
#define INTERNALREPORTER_H

#include "configParser.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Reporter
{

    Reporter(const std::string &folder);
    void report_meta(const std::vector<double> &params);

    void report_meta(std::string meta);

    void report_simple_mutation(uint iteration_number, const std::string &filename);

    template <typename X, typename Y>
    void report_data(X x, Y y, const std::string &filename)
    {
        auto it = res_fout_poll.find(filename);
        if (it != res_fout_poll.end())
        {
            std::ofstream *fout = it->second;
            *fout << std::setprecision(std::numeric_limits<double>::max_digits10) << x << " " << y
                  << std::endl;
            return;
        }
    }

    void create_log_file(std::string x_sign, std::string y_sign, const std::string &filename);

    void create_log_file_simple_mutation(std::string filename);

    void create_meto_file(std::string filename);

    void close_meto_file();

    ~Reporter();

    static std::shared_ptr<Reporter> createReporter(AlgorithmsTags alg_tag, uint32_t lambda,
                                                    uint32_t n, const std::vector<double> *params);

private:
    uint prev_iteration_number = 0;
    uint cnt_simple_mutation = 0;
    std::string folder;
    std::unordered_map<std::string, std::ofstream *> res_fout_poll;
    std::ofstream meto_fout;
};

#endif // INTERNALREPORTER_H
