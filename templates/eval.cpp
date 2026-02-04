#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// Usage: ./eval <executable> <input_dir> [output_dir] [validator]

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0]
         << " <executable> <input_dir> [output_dir] [validator]" << endl;
    return 1;
  }

  string executable = argv[1];
  string input_dir = argv[2];
  string output_dir = (argc > 3) ? argv[3] : "";
  string validator = (argc > 4) ? argv[4] : "";

  if (executable.substr(0, 2) != "./" && executable[0] != '/')
    executable = "./" + executable;
  if (!validator.empty() && validator.substr(0, 2) != "./" &&
      validator[0] != '/')
    validator = "./" + validator;

  // Auto-detect validator if not provided and no output directory
  if (validator.empty() && output_dir.empty()) {
    if (fs::exists("validator")) {
      validator = "./validator";
      cout << "Auto-detected validator: ./validator" << endl;
    }
  }

  vector<fs::path> inputs;
  for (const auto& entry : fs::directory_iterator(input_dir)) {
    if (entry.path().extension() == ".in" || entry.path().extension().empty()) {
      inputs.push_back(entry.path());
    }
  }
  sort(inputs.begin(), inputs.end());

  double max_time = 0;

  bool has_input_validator = fs::exists("input_validator");

  for (const auto& input_path : inputs) {
    cout << "Test " << input_path.filename() << ": ";

    if (has_input_validator) {
      string val_cmd = "./input_validator < \"" + input_path.string() +
                       "\" > /dev/null 2>&1";
      int val_ret = system(val_cmd.c_str());
      if (val_ret != 0) {
        cout << "INVALID INPUT" << endl;
        continue;
      }
    }

    string tmp_output = "temp.out";
    string command =
        executable + " < \"" + input_path.string() + "\" > " + tmp_output;

    auto start = chrono::high_resolution_clock::now();
    int ret = system(command.c_str());
    auto end = chrono::high_resolution_clock::now();

    if (ret != 0) {
      cout << "RTE ";
    }

    chrono::duration<double> diff = end - start;
    double secs = diff.count();
    if (secs > max_time) max_time = secs;

    string verdict = "OK";

    if (!validator.empty()) {
      string val_cmd =
          validator + " \"" + input_path.string() + "\" " + tmp_output;
      int val_ret = system(val_cmd.c_str());
      if (val_ret != 0) verdict = "WA (Validator)";
    } else if (!output_dir.empty()) {
      fs::path expected_out = fs::path(output_dir) / input_path.filename();
      if (!fs::exists(expected_out)) {
        expected_out = fs::path(output_dir) / input_path.stem();
        expected_out += ".out";
      }

      if (fs::exists(expected_out)) {
        string diff_cmd = "diff -w " + tmp_output + " \"" +
                          expected_out.string() + "\" > /dev/null";
        int diff_ret = system(diff_cmd.c_str());
        if (diff_ret != 0) verdict = "WA";
      } else {
        verdict = "SKIP (No output file)";
      }
    } else {
      verdict = "RUN (No check)";
    }

    cout << verdict << " [" << secs << "s]" << endl;
  }

  cout << "\nMax Run Time: " << max_time << "s" << endl;
  fs::remove("temp.out");

  return 0;
}
