#include "genlib.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }

    auto p = gen_permutation(10);
    for (int x : p) {
        cout << x << " ";
    }
    cout << endl;
    
    return 0;
}
