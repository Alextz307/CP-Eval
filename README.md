# CP-Eval

CP-Eval is a local competitive programming evaluation and test generation tool written in C++. It helps you verify your solutions against brute-force implementations using stress testing and evaluate them against local test suites.

## Features

- **Problem Generator**: Quickly set up a new problem environment with templates.
- **Stress Testing**: Run an infinite loop comparing your solution against a brute-force approach (or others) using a generator.
- **Evaluation**: Run your solution against a directory of input files, check outputs (diff or custom validator), and measure execution time.
- **Input Validation**: Automatically check if your input files satisfy problem constraints using a custom validator.
- **Customizable**: Templates for `gen`, `main`, `brute` are included.
- **Generator Library**: `genlib.hpp` includes helpers for Arrays, Permutations, Trees (Prüfer), and Graphs (Connected & General).

## Installation

1. Clone or download this repository.
2. Build the tool:
   ```bash
   ./install.sh
   ```
   This creates the `cp-eval` executable.

## Usage

### 1. Create a New Problem
```bash
./cp-eval ProblemName
cd ProblemName
```
This creates a folder `ProblemName` with:
- `main.cpp`: Your solution.
- `brute.cpp`: Brute-force solution (for stress testing).
- `gen.cpp`: Generic test case generator.
- `genlib.hpp`: Generator library header (contains `genTree`, `genConnectedGraph`, `genGraph`, etc.).
- `stress_test.cpp` & `eval.cpp`: Comparison tools.
- `compile.sh`: Script to compile everything.

### 2. Compile
Inside the problem directory:
```bash
./compile.sh
```

### 3. Stress Testing
To stress test your solution (`main`) against the brute force (`brute`) using the generator (`gen`):
```bash
./stress gen main brute
```
You can add more executables to compare if needed.
If a mismatch is found, it prints the input and the differing outputs, and saves the input to `input.in`.

Additionally, if an `input_validator` executable is present, `stress` will run it on every generated input. If validation fails, the stress tester stops and reports the invalid input.

### 4. Evaluation
To run your solution against a test suite:
1. Place input files in an `input/` directory (e.g., `input/1.in`, `input/2.in`).
2. (Optional) Place expected output files in an `output/` directory (e.g., `output/1.out`).
3. Run:
```bash
# Check against output files
./eval main input output

# OR check using a custom validator (if no output files available or needed)
./eval main input "" validator

# OR let it auto-detect ./validator
# If you didn't specify an output dir, and ./validator exists, it will use it.
./eval main input
```
(Note: A default `validator.cpp` template is included. You should edit it to implement your problem-specific checking logic).

### 5. Input Validation
To ensure your input files meet problem constraints:
1. Edit `input_validator.cpp` (template included) to return `0` for valid input and non-zero for invalid input.
2. Compile it (done automatically by `.compile.sh`).
3. Run:
```bash
./check_inputs input
```
This tool checks all files in the `input/` directory and reports any inputs that fail validation.

Additionally, `./eval` will automatically look for `./input_validator`. If it exists, it will validate each input file before running your solution, skipping any invalid inputs.


## Project Structure
- `src/cp_setup.cpp`: Source code for the problem generator.
- `templates/`: Directory containing source templates (`main.cpp`, `stress_test.cpp`, etc.).
- `install.sh`: Build script that compiles the tool and links the templates.
- `run_tests.py`: Python script to verify the tool functionality.
- `ProblemName/`: Generated problem directory (self-contained, copies templates).

## Git Configuration

To keep your repository clean, the `.gitignore` is configured to:
- **Ignore all root-level directories** except `src/`, `templates/`, and `tests/`. This means any new problem directory you create (e.g., `./Codeforces123/`) will **not** be tracked by git.
- **Ignore build artifacts**: Executables like `cp-eval`, `main`, `brute`, `gen`, etc., are ignored.
- **Ignore temporary files**: `.tmp` files, input/output files (`input.in`, `out_main`), and generator outputs are ignored.

You can freely create problem workspaces in the root directory without worrying about accidentally committing them.

## Requirements
- GCC (g++) supporting C++17.
- Python 3.12 or higher.
- Unix-like environment (Linux/macOS) **or WSL (Windows Subsystem for Linux)** with bash.

## Tool Development & Testing

To verify the tool functionality (e.g., after modifying templates or source code), run the test suite:

```bash
./run_tests.py
```

This runs a comprehensive suite of tests covering:
- Stress testing logic (detection of WA).
- Evaluation logic (output diffing and custom validators).
- Generator compilation and execution.
- Input validation integration.
