#include <iostream>
#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }
    
    if (randInt(1, 5) == 1) {
        cout << "INVALID" << endl;
    } else {
        cout << "VALID" << endl;
    }

    return 0;
}
