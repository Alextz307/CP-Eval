#include <iostream>
#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }
    
    // Default: Print a single random integer
    int n = rand_int(1, 100);
    cout << n << endl;

    return 0;
}
