#include "collector.h"

void Collector::print_map(std::string_view comment, const std::map<std::string, std::string>& m)
{
    std::cout << comment;
    // Iterate using C++17 facilities
    for (const auto& [key, value] : m)
        std::cout << '[' << key << "] = " << value << "; ";
        
    std::cout << '\n';
}

std::string Collector::get_filename(const fs::path& p) { return p.filename().string(); }

Collector::FileCollector::FileCollector()
{
    std::string path = "";
    path.append(fs::current_path().string());
    path.append("\\site");

    for (const auto & entry : fs::directory_iterator(path))
    {
        if (fs::is_directory(entry.symlink_status()))
        {
            // Is dir, now not handle
        }
        else
        {
            std::ifstream t(entry.path().c_str(), std::ios::in);
            std::stringstream buffer;
            std::string fileName = get_filename(entry.path());
            buffer << t.rdbuf();
            fileData[fileName.substr(fileName.find_last_of("\\") + 1)] = buffer.str();
        }
    }

    // print_map("FileData: ", fileData);
}

std::string Collector::FileCollector::get_file_data(std::string s)
{
    if (auto search = fileData.find(s); search != fileData.end())
        return (fileData[s].c_str());
    else
        return std::string();
}
