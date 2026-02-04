#include "genlib.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  if (argc > 1) {
    rng.seed(stoi(argv[1]));
  }

  vector<int> a = genArray(10, 1, 20);

  for (const int& x : a) {
    cout << x << " ";
  }
  cout << endl;

  return 0;
}
