#include "algorithmTags.h"
#include <string>

AlgorithmsTags str_to_algorithm_tag(const std::string &s)
{
    if (s == "St")
        return AlgorithmsTags::StaticParamsTag;
    else if (s == "A2")
        return AlgorithmsTags::AdjustingParamsTwoSubpopulationsTag;
    else if (s == "A3")
        return AlgorithmsTags::AdjustingParamsThreeSubpopulationsTag;
    else if (s == "ThStatic")
        return AlgorithmsTags::TheoryStaticParamsTag;
    return AlgorithmsTags::UndefinedAlgorithmTag;
}

std::string algorithm_tag_to_str(AlgorithmsTags tag)
{
    switch (tag)
    {
    case AlgorithmsTags::StaticParamsTag:
        return "St - StaticParams";
    case AlgorithmsTags::AdjustingParamsThreeSubpopulationsTag:
        return "A3 - AdjustingParamsThreeSubpopulations";
    case AlgorithmsTags::AdjustingParamsTwoSubpopulationsTag:
        return "A2 - AdjustingParamsTwoSubpopulations";
    case AlgorithmsTags::TheoryStaticParamsTag:
        return "ThStatic - TheoryStaticParams";
    default:
        return "Undefined algorithm";
    }
}
