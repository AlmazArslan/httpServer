#include <filesystem>
#include <iostream>
#include <string>
#include <sys/stat.h>

namespace fs = std::filesystem;
std::string get_stem(const fs::path& p) { return p.filename().string(); }

int main()
{
    std::string path = "";
    path.append(fs::current_path().string());
    path.append("\\site");

    std::cout << path << std::endl;

    for (const auto & entry : fs::directory_iterator(path))
    {
        std::string fileName = get_stem(entry.path());
        std::cout << fileName << std::endl;
    }
}