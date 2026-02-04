#include "genlib.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }

    genConnectedGraph(5, 7);
    
    return 0;
}
