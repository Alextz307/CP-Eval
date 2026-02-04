#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

// Usage: ./check_inputs [input_dir] [validator_executable]

int main(int argc, char* argv[]) {
  string input_dir = (argc > 1) ? argv[1] : "./input";
  string validator = (argc > 2) ? argv[2] : "./input_validator";

  if (validator.substr(0, 2) != "./" && validator[0] != '/')
    validator = "./" + validator;

  if (!fs::exists(validator)) {
    cerr << "Validator executable not found: " << validator << endl;
    cerr << "Compile it first: g++ input_validator.cpp -o input_validator"
         << endl;
    return 1;
  }

  if (!fs::exists(input_dir)) {
    cerr << "Input directory not found: " << input_dir << endl;
    return 1;
  }

  vector<fs::path> inputs;
  for (const auto& entry : fs::directory_iterator(input_dir)) {
    if (entry.path().extension() == ".in" || entry.path().extension().empty()) {
      inputs.push_back(entry.path());
    }
  }
  sort(inputs.begin(), inputs.end());

  int failed = 0;
  int passed = 0;

  cout << "Checking " << inputs.size() << " inputs with " << validator << "..."
       << endl;

  for (const auto& input_path : inputs) {
    string command =
        validator + " < \"" + input_path.string() + "\" > /dev/null 2>&1";
    int ret = system(command.c_str());

    if (ret == 0) {
      // cout << "[PASS] " << input_path.filename() << endl;
      passed++;
    } else {
      cout << "[FAIL] " << input_path.filename() << " (Exit Code: " << ret
           << ")" << endl;
      failed++;
    }
  }

  cout << "\nSummary: " << passed << " Passed, " << failed << " Failed."
       << endl;

  return (failed > 0 ? 1 : 0);
}
