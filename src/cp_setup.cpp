#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

#ifndef TEMPLATE_DIR
#define TEMPLATE_DIR "./templates"
#endif

void copyTemplate(const std::string& filename, const fs::path& destDir) {
    fs::path source = fs::path(TEMPLATE_DIR) / filename;
    fs::path dest = destDir / filename;

    try {
        fs::copy_file(source, dest, fs::copy_options::overwrite_existing);
    } catch (fs::filesystem_error& e) {
        std::cerr << "Error copying " << filename << ": " << e.what() << std::endl;
        // Proceeding anyway, but this is critical
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <problem_name>" << std::endl;
        return 1;
    }

    std::string problemName = argv[1];
    if (fs::exists(problemName)) {
        std::cerr << "Directory " << problemName << " already exists." << std::endl;
        return 1;
    }

    try {
        fs::create_directory(problemName);
        fs::path problemPath = fs::path(problemName);

        std::vector<std::string> templates = {
            "main.cpp",
            "brute.cpp",
            "gen.cpp",
            "input_validator.cpp",
            "check_inputs.cpp",
            "genlib.hpp",
            "stress.cpp",
            "eval.cpp",
            "validator.cpp",
            "compile.sh"
        };

        for (const auto& t : templates) {
            copyTemplate(t, problemPath);
        }

        fs::permissions(problemPath / "compile.sh", 
            fs::perms::owner_exec | fs::perms::owner_read | fs::perms::owner_write);

        std::cout << "Created problem environment: " << problemName << std::endl;
        std::cout << "Run 'cd " << problemName << " && ./compile.sh' to get started." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
