#include <metrics.h>


int64_t utils::metrics1(const ResultsTable &t1, const ResultsTable &t2, uint lambda)
{
    return metrics1(t1.getMapFromNToRes(lambda), t2.getMapFromNToRes(lambda));
}

int64_t utils::metrics1(const std::unordered_map<uint, uint> &m1,
                        const std::unordered_map<uint, uint> &m2)

{
    int64_t ans = 0;
    for (const auto &itn1 : m1)
    {
        auto it2 = m2.find(itn1.first);
        ans += static_cast<int64_t>(itn1.second) - static_cast<int64_t >(it2->second);
    }
    return ans;
}


