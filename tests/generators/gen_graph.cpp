#include "genlib.hpp"

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }

    gen_connected_graph(5, 7);
    
    return 0;
}
