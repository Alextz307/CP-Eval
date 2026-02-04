#include <iostream>

#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc > 1) {
    rng.seed(stoi(argv[1]));
  }

  cout << 10 << endl;
  genTree(10);

  return 0;
}
