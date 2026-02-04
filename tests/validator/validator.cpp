#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
  ifstream ss(argv[2]);
  string s;
  ss >> s;

  if (s == "B") {
    return 0;
  }

  return 1;
}
