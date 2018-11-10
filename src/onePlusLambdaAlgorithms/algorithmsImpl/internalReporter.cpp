#include "internalReporter.h"
#include "algorithmTags.h"

Reporter::Reporter(const std::string &folder) : folder(folder) {}

void Reporter::report_meta(const std::vector<double> &params)
{
    int i = 1;
    for (double p : params)
    {
        meto_fout << "param " + std::to_string(i) + ": " + std::to_string(p) << std::endl;
        i++;
    }
}

void Reporter::report_meta(std::string meta) { meto_fout << std::move(meta) << std::endl; }

void Reporter::create_log_file(std::string x_sign, std::string y_sign, const std::string &filename)
{
    std::ofstream *fout = new std::ofstream(folder + "/" + filename);
    res_fout_poll[filename] = fout;
    *fout << x_sign.c_str() << " " << y_sign << std::endl;
}

std::shared_ptr<Reporter> Reporter::createReporter(AlgorithmsTags alg_tag, uint32_t lambda,
                                                   uint32_t n, const std::vector<double> *params)
{
    namespace fs = boost::filesystem;
    static int folder_count = 0;
    std::string folder_name = "data" + std::to_string(folder_count);
    ++folder_count;
    fs::path p = fs::path(folder_name);
    bool ok = fs::exists(p);
    if (ok)
    {
        for (fs::directory_iterator end_dir_it, it(p); it != end_dir_it; ++it)
        {
            fs::remove_all(it->path());
        }
    }
    else
    {
        fs::create_directory(p);
    }

    std::shared_ptr<Reporter> reporter_ptr = std::make_shared<Reporter>(folder_name);
    std::string meto_file_name = "info";
    reporter_ptr->create_meto_file(meto_file_name);
    std::string meto_info = "Lambda: " + std::to_string(lambda) +
                            "\nOffspringLength: " + std::to_string(n) +
                            "\nUsedAlgorithm: " + algorithm_tag_to_str(alg_tag) + "\n";
    reporter_ptr->report_meta(meto_info);
    if (params != nullptr)
        reporter_ptr->report_meta(*params);
    reporter_ptr->close_meto_file();
    return reporter_ptr;
}

void Reporter::create_meto_file(std::string filename)
{
    if (meto_fout.is_open())
    {
        meto_fout.close();
    }
    meto_fout.open(folder + "/" + filename);
}

void Reporter::close_meto_file() { meto_fout.close(); }

Reporter::~Reporter()
{
    for (const auto &so : res_fout_poll)
    {
        delete so.second;
    }
    if (meto_fout.is_open())
    {
        meto_fout.close();
    }
}
