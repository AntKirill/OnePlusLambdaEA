#include "chartBuilder.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

using namespace std;

void printUsage()
{
    std::cout << "Usage:\n"
              << "./binary_name filename_with_xy" << std::endl;
}

ChartBuilder *builder = nullptr;

int buildGraph(const char *name)
{
    if (builder == nullptr)
        return 1;
    ifstream fin(name);
    if (!fin.good())
    {
        return 1;
    }
    string x_sign, y_sign;
    fin >> x_sign >> y_sign;
    double x, y;
    std::vector<double> xs, ys;
    while (fin >> x >> y)
    {
        xs.push_back(x);
        ys.push_back(y);
    }
    builder->set_x_name(x_sign.c_str());
    builder->set_y_name(y_sign.c_str());
    builder->addNewXYGraph(xs, ys, name);
    builder->saveGraphPng(name);
    builder->removeAllGraphs();
    return 0;
}

bool copyDir(boost::filesystem::path const &source, boost::filesystem::path const &destination)
{
    namespace fs = boost::filesystem;
    try
    {
        // Check whether the function call is valid
        if (!fs::exists(source) || !fs::is_directory(source))
        {
            std::cerr << "Source directory " << source.string()
                      << " does not exist or is not a directory." << '\n';
            return false;
        }
        if (fs::exists(destination))
        {
            std::cerr << "Destination directory " << destination.string() << " already exists."
                      << '\n';
            return false;
        }
        // Create the destination directory
        if (!fs::create_directory(destination))
        {
            std::cerr << "Unable to create destination directory" << destination.string() << '\n';
            return false;
        }
    }
    catch (fs::filesystem_error const &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    // Iterate through the source directory
    for (fs::directory_iterator file(source); file != fs::directory_iterator(); ++file)
    {
        try
        {
            fs::path current(file->path());
            if (fs::is_directory(current))
            {
                // Found directory: Recursion
                if (!copyDir(current, destination / current.filename()))
                {
                    return false;
                }
            }
            else
            {
                // Found file: Copy
                fs::copy_file(current, destination / current.filename());
            }
        }
        catch (fs::filesystem_error const &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return true;
}

void removeAllInsideDir(boost::filesystem::path const &source)
{
    namespace fs = boost::filesystem;
    for (fs::directory_iterator end_dir_it, it(source); it != end_dir_it; ++it)
    {
        fs::remove_all(it->path());
    }
}

void walkFs(std::vector<std::string> files, std::string resultsDirName, int sampleRate)
{
    namespace fs = boost::filesystem;
    // prepare results folder
    fs::path resPath(resultsDirName);
    if (!fs::exists(resPath))
    {
        fs::create_directory(resPath);
    }
    else
    {
        removeAllInsideDir(resPath);
    }

    // building charts for some data's
    std::string data("./data");
    fs::path p = ".";
    int cnt = 0;
    for (fs::directory_iterator end_dir_it, it(p); it != end_dir_it; ++it)
    {
        std::string dir_name = it->path().string();
        if (dir_name.size() < 6)
            continue;
        std::string start_dir_name = dir_name.substr(0, 6);
        if (start_dir_name == data)
        {
            if (cnt % sampleRate == 0)
            {
                int ok = 0;
                std::string copyOfDataDir = resultsDirName + "/" + dir_name;
                fs::path copyOfDataPath(copyOfDataDir);
                if (fs::exists(copyOfDataPath))
                {
                    fs::remove_all(copyOfDataPath);
                }
                copyDir(it->path(), copyOfDataPath);
                for (auto &file : files)
                {
                    std::string full_file_name = copyOfDataDir + "/" + file;
                    ok += buildGraph(full_file_name.c_str());
                }
                if (ok == 0)
                    cout << "build charts for directory: " << copyOfDataDir << endl;
                else
                    cnt--;
            }
            cnt++;
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    if (argc == 1)
//    {
//        printUsage();
//        return 1;
//    }
    //    if (buildGraph(argv[1]))
    //    {
    //        return 1;
    //    }
    builder = new ChartBuilder;
    walkFs({"max_fitness", "probability", "easy_mutations"}, "RandomSamplesData", 10);
    delete builder;
    return 0;
}
