#include "iostream"
#include "configParser.h"
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    string info = "info.txt";
    string configs = "configs.txt";
    if (argc == 3)
    {
        info = argv[1];
        configs = argv[2];
    }
    else if (argc == 2)
    {
        configs = argv[1];
    }
    ConfigParser parser(configs);
    std::shared_ptr<const ParsedParams> params = parser.parse();
    ofstream fout(info);
    fout << "selfAdjParams: ";
    for (auto i : params->selfAdjParams) fout << i << " ";
    fout << endl;
}
