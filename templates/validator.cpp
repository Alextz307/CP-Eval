#include <fstream>
#include <iostream>

using namespace std;

// Usage: ./validator <input_file> <user_output_file>
// Returns 0 if valid, 1 if invalid.

int main(int argc, char* argv[]) {
  if (argc < 3) {
    cerr << "Usage: " << argv[0] << " <input_file> <user_output_file>" << endl;
    return 1;
  }

  ifstream input(argv[1]);
  ifstream output(argv[2]);

  if (!input || !output) {
    cerr << "Error opening files." << endl;
    return 1;
  }

  // --- WRITE VALIDATOR LOGIC HERE ---
  // Example: Check if the output number is the square of the input number
  /*
      int n; input >> n;
      int ans; output >> ans;
      if (ans != n * n) {
          return 1;
      }
      return 0; // OK
  */

  return 0;
}
