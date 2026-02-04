#!/bin/bash
# Test Suite for CP-Eval

PROB="TestSuiteProb"
echo "Cleaning up any previous test..."
rm -rf $PROB

echo "1. Generating Problem..."
./cp-eval $PROB
if [ ! -d "$PROB" ]; then
    echo "FAILED: Problem directory not created."
    exit 1
fi

cd $PROB
echo "2. Compiling Tools..."
./compile.sh > /dev/null 2>&1
if [ ! -f stress ] || [ ! -f eval ]; then
    echo "FAILED: Tools not compiled."
    exit 1
fi

echo "3. Verifying Stress Test (Expected Failure)..."
# Modify main and brute to be different
echo "#include <iostream>
using namespace std;
int main() { cout << \"A\" << endl; return 0; }" > main.cpp
echo "#include <iostream>
using namespace std;
int main() { cout << \"B\" << endl; return 0; }" > brute.cpp

g++ main.cpp -o main
g++ brute.cpp -o brute

./stress gen main brute > stress_out.tmp 2>&1
if ! grep -q "FAILED" stress_out.tmp; then
    echo "FAILED: Stress test did not catch mismatch."
    cat stress_out.tmp
    exit 1
fi
echo "Stress test working as expected (caught bug)."

echo "4. Verifying Evaluator..."
# Fix main to match output
mkdir input output
echo "1" > input/1.in
echo "A" > output/1.out
./eval main input output > eval_out.tmp 2>&1

if ! grep -q "OK" eval_out.tmp; then
    echo "FAILED: Evaluator did not report OK."
    cat eval_out.tmp
    exit 1
fi
echo "Evaluator (output dir) working as expected."

echo "5. Verifying Auto-Validator..."
# Inject a validator that enforces output to be "B"
echo "#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main(int argc, char* argv[]) {
    ifstream bfs(argv[2]);
    string s; bfs >> s;
    if (s == \"B\") return 0;
    return 1;
}
" > validator.cpp
g++ validator.cpp -o validator

# ./eval main input should now use ./validator automatically
# main currently prints "A". Validator expects "B".
# So validator should FAIL.

./eval main input > eval_val_fail.tmp 2>&1
if ! grep -q "WA (Validator)" eval_val_fail.tmp; then
    echo "FAILED: Auto-validator did not detect mismatch."
    cat eval_val_fail.tmp
    exit 1
fi

# Now make main print "B" to match brute
echo "#include <iostream>
using namespace std;
int main() { cout << \"B\" << endl; return 0; }" > main.cpp
g++ main.cpp -o main

./eval main input > eval_val_pass.tmp 2>&1
if ! grep -q "OK" eval_val_pass.tmp; then
    echo "FAILED: Auto-validator did not report OK on match."
    cat eval_val_pass.tmp
    exit 1
fi
echo "Auto-Validator working as expected."

echo "6. Verifying Generator Library..."
# Test Graph Generation
echo "#include \"../templates/genlib.hpp\"
int main(int argc, char* argv[]) {
    if (argc > 1) rng.seed(stoi(argv[1]));
    gen_connected_graph(5, 7);
    return 0;
}" > gen.cpp
g++ -std=c++17 gen.cpp -o gen
./gen 123 > gen_out.tmp
if [ ! -s gen_out.tmp ]; then
    echo "FAILED: gen_connected_graph produced empty output."
    exit 1
fi
echo "Graph generation verified."

# Test Permutation Generation
echo "#include \"../templates/genlib.hpp\"
int main(int argc, char* argv[]) {
    if (argc > 1) rng.seed(stoi(argv[1]));
    auto p = gen_permutation(10);
    for (int x : p) cout << x << \" \";
    cout << endl;
    return 0;
}" > gen.cpp
g++ -std=c++17 gen.cpp -o gen
./gen 123 > gen_perm.tmp
if [ ! -s gen_perm.tmp ]; then
    echo "FAILED: gen_permutation produced empty output."
    exit 1
fi
echo "Permutation generation verified."

cd ..
rm -rf $PROB
echo "ALL TESTS PASSED."
