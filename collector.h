#ifndef COLLECTOR
#define COLLECTOR

#include <cstdlib>
#include <memory>
#include <string>
#include <iostream>
#include <cstring>
#include <map>
#include <filesystem>
#include <fstream>

namespace Collector
{
    namespace fs = std::filesystem;
    void print_map(std::string_view comment, const std::map<std::string, std::string>& m);
    std::string get_filename(const fs::path& p);
    class FileCollector
    {
    public:
        FileCollector();
        std::string get_file_data(std::string s);
    private:
        std::map<std::string, std::string> fileData;
    };
}

#endif