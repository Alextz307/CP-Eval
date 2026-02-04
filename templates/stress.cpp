#include <iostream>
#include <vector>
#include <string>

#include <cstdlib>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <generator> <executable1> <executable2> ...\n";
        return 1;
    }

    string generator = argv[1];
    vector<string> executables;
    for (int i = 2; i < argc; ++i) {
        executables.emplace_back(argv[i]);
    }

    int testCase = 1;
    while (true) {
        string cmdGen = "./" + generator + " " + to_string(testCase) + " > input.in";
        if (system(cmdGen.c_str()) != 0) {
            cerr << "Generator failed at seed " << testCase << endl;
            return 1;
        }

        vector<string> outputs;
        bool allSame = true;
        
        cout << "Test " << testCase << ": ";

        for (const auto& exe : executables) {
            string outFile = exe + ".out";
            string cleanName = exe;
            if (cleanName.substr(0, 2) == "./") {
                cleanName = cleanName.substr(2);
            }
            outFile = cleanName + ".tmp_out";
            
            string cmdRun = "./" + cleanName + " < input.in > " + outFile;
            if (system(cmdRun.c_str()) != 0) {
                 cerr << "Runtime error in " << exe << endl;
                 return 1;
            }
            
            ifstream ifs(outFile);
            string content((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
            outputs.emplace_back(content);
        }

        for (size_t i = 1; i < outputs.size(); ++i) {
            if (outputs[i] != outputs[0]) {
                allSame = false;
                break;
            }
        }

        if (!allSame) {
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
            cout << "OK" << endl;
        }

        testCase += 1;
    }

    return 0;
}
