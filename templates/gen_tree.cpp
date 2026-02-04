#include <iostream>
#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }
    
    int n = rand_int(2, 20); // Random size
    cout << n << endl;
    gen_tree(n);

    return 0;
}
