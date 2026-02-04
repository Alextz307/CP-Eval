#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <fstream>

using namespace std;

// Helper to run a command and capture output is tricky with just system().
// For simplicity in this template, we will use system() and temporary files.
// Enhancements: using popen or pipe for direct capture.

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <generator> <executable1> <executable2> ...\n";
        return 1;
    }

    string generator = argv[1];
    vector<string> executables;
    for (int i = 2; i < argc; ++i) {
        executables.push_back(argv[i]);
    }

    int test_case = 1;
    while (true) {
        // Generate test
        string cmd_gen = "./" + generator + " " + to_string(test_case) + " > input.in";
        if (system(cmd_gen.c_str()) != 0) {
            cerr << "Generator failed at seed " << test_case << endl;
            return 1;
        }

        vector<string> outputs;
        bool all_same = true;
        
        cout << "Test " << test_case << ": ";

        for (const auto& exe : executables) {
            string out_file = exe + ".out";
            // Strip ./ if present for filename
            string clean_name = exe;
            if (clean_name.substr(0, 2) == "./") clean_name = clean_name.substr(2);
            out_file = clean_name + ".tmp_out";
            
            string cmd_run = "./" + clean_name + " < input.in > " + out_file;
            if (system(cmd_run.c_str()) != 0) {
                 cerr << "Runtime error in " << exe << endl;
                 return 1;
            }
            
            // Read output
            ifstream ifs(out_file);
            string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            outputs.push_back(content);
        }

        // Compare all to the first one
        for (size_t i = 1; i < outputs.size(); ++i) {
            if (outputs[i] != outputs[0]) {
                all_same = false;
                break;
            }
        }

        if (!all_same) {
            cout << "FAILED\n";
            cout << "--- Input ---\n";
            ifstream in("input.in");
            cout << in.rdbuf() << endl;
            
            cout << "--- Outputs ---\n";
            for (size_t i = 0; i < executables.size(); ++i) {
                cout << "[" << executables[i] << "]:\n" << outputs[i] << "\n"; 
            }
            return 1;
        } else {
            cout << "OK" << endl; // Maybe overwrite line or just minimal output
        }

        test_case++;
    }

    return 0;
}
