#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc > 1) {
        rng.seed(stoi(argv[1]));
    }
    
    int n = rand_int(5, 10);
    int m = rand_int(n - 1, n * (n - 1) / 2);
    
    gen_connected_graph(n, m);

    return 0;
}
