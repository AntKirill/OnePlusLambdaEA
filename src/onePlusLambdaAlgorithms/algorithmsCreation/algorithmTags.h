#ifndef ALGORITHMTAGS_H
#define ALGORITHMTAGS_H

#include <string>

enum AlgorithmsTags
{
    AdjustingParamsTwoSubpopulationsTag,
    AdjustingParamsThreeSubpopulationsTag,
    StaticParamsTag,
    TheoryStaticParamsTag,
    UndefinedAlgorithmTag
};

AlgorithmsTags str_to_algorithm_tag(const std::string &s);
std::string algorithm_tag_to_str(AlgorithmsTags tag);

#endif // ALGORITHMTAGS_H
