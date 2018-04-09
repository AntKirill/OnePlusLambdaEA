#ifndef METRICS_H
#define METRICS_H

#include "resultsTable.h"

namespace utils {
    int64_t metrics1(const ResultsTable &, const ResultsTable &, uint lambda);
    int64_t metrics1(const std::unordered_map<uint, uint> &, const std::unordered_map<uint, uint> &);
}

#endif // METRICS_H
