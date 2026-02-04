#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

#ifndef TEMPLATE_DIR
#define TEMPLATE_DIR "./templates"
#endif

void copy_template(const std::string& filename, const fs::path& dest_dir) {
    fs::path source = fs::path(TEMPLATE_DIR) / filename;
    fs::path dest = dest_dir / filename;

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

    std::string problem_name = argv[1];
    if (fs::exists(problem_name)) {
        std::cerr << "Directory " << problem_name << " already exists." << std::endl;
        return 1;
    }

    try {
        fs::create_directory(problem_name);
        fs::path problem_path = fs::path(problem_name);

        std::vector<std::string> templates = {
            "main.cpp",
            "brute.cpp",
            "gen.cpp",
            "gen_tree.cpp",
            "gen_graph.cpp",
            "genlib.hpp",
            "stress.cpp",
            "eval.cpp",
            "validator.cpp",
            "compile.sh"
        };

        for (const auto& t : templates) {
            copy_template(t, problem_path);
        }

        // Make compile script executable
        fs::permissions(problem_path / "compile.sh", 
            fs::perms::owner_exec | fs::perms::owner_read | fs::perms::owner_write);

        std::cout << "Created problem environment: " << problem_name << std::endl;
        std::cout << "Run 'cd " << problem_name << " && ./compile.sh' to get started." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
