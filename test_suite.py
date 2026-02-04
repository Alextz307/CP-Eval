#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess
from pathlib import Path

# Configuration
PROB_NAME = "TestSuiteProb"
TESTS_DIR = Path("../tests")
RED = "\033[0;31m"
GREEN = "\033[0;32m"
NC = "\033[0m"


def log(msg: str) -> None:
    print(f"{GREEN}[TEST SUITE] {msg}{NC}")


def error(msg: str) -> None:
    print(f"{RED}[FAILED] {msg}{NC}")
    sys.exit(1)


def run_command(cmd_list: list[str], cwd: Path | None = None, capture: bool = True) -> subprocess.CompletedProcess:
    try:
        result = subprocess.run(
            cmd_list,
            cwd=cwd,
            capture_output=capture,
            text=True,
            check=False
        )
        return result
    except Exception as e:
        error(f"Execution failed: {cmd_list} -> {e}")
        # Unreachable, but satisfies type checker
        sys.exit(1)


def setup_env() -> None:
    log("Setting up fresh environment...")
    
    # Ensure we are in project root or correct subdir
    if Path("install.sh").exists():
        # In root
        pass
    elif Path("../install.sh").exists():
        os.chdir("..")
    
    root_path = Path.cwd()
    prob_path = root_path / PROB_NAME
    
    if prob_path.exists():
        shutil.rmtree(prob_path)
    
    cp_eval_bin = root_path / "cp-eval"
    if not cp_eval_bin.exists():
        error("cp-eval binary not found. Run ./install.sh first.")
    
    # Create problem
    run_command([str(cp_eval_bin), PROB_NAME], capture=True)
    
    if not prob_path.exists():
        error("Problem directory not created.")
    
    os.chdir(prob_path)
    
    # Compile tools
    if not Path("compile.sh").exists():
        error("compile.sh not found in problem directory.")
        
    res = run_command(["./compile.sh"])
    if res.returncode != 0:
        error("Tools compilation failed.")


def run_test_stress_fail() -> None:
    log("Running Stress Test (Expected Failure)...")
    
    # Copy source files
    shutil.copy(TESTS_DIR / "stress_fail/main.cpp", ".")
    shutil.copy(TESTS_DIR / "stress_fail/brute.cpp", ".")
    
    # Compile
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    run_command(["g++", "-O2", "-std=c++17", "brute.cpp", "-o", "brute"])
    
    # Run stress
    res = run_command(["./stress", "gen", "main", "brute"])
    
    if "FAILED" not in res.stdout and "FAILED" not in res.stderr:
        print(res.stdout)
        print(res.stderr)
        error("Stress test did not catch mismatch.")


def run_test_eval_basic() -> None:
    log("Running Basic Eval Test...")
    
    shutil.copy(TESTS_DIR / "eval_basic/main.cpp", ".")
    
    input_dir = Path("input")
    output_dir = Path("output")
    input_dir.mkdir(exist_ok=True)
    output_dir.mkdir(exist_ok=True)
    
    # Copy directory contents
    for f in (TESTS_DIR / "eval_basic/input").glob("*"):
        shutil.copy(f, input_dir)
    for f in (TESTS_DIR / "eval_basic/output").glob("*"):
        shutil.copy(f, output_dir)
        
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    
    res = run_command(["./eval", "main", "input", "output"])
    
    if "OK" not in res.stdout:
        print(res.stdout)
        error("Evaluator did not report OK.")


def run_test_validator() -> None:
    log("Running Validator Test...")
    
    shutil.copy(TESTS_DIR / "validator/validator.cpp", ".")
    run_command(["g++", "-O2", "-std=c++17", "validator.cpp", "-o", "validator"])
    
    # 1. Test Failure
    shutil.copy(TESTS_DIR / "validator/main_fail.cpp", "main.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    
    res_fail = run_command(["./eval", "main", "input"])
    if "WA (Validator)" not in res_fail.stdout:
        print(res_fail.stdout)
        error("Auto-validator did not detect mismatch.")
        
    # 2. Test Success
    shutil.copy(TESTS_DIR / "validator/main_pass.cpp", "main.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    
    res_pass = run_command(["./eval", "main", "input"])
    if "OK" not in res_pass.stdout:
        print(res_pass.stdout)
        error("Auto-validator did not report OK on match.")


def run_test_generators() -> None:
    log("Running Generator Tests...")
    
    # Graph
    shutil.copy(TESTS_DIR / "generators/gen_graph.cpp", "gen.cpp")
    run_command(["g++", "-std=c++17", "gen.cpp", "-o", "gen"])
    
    res_graph = run_command(["./gen", "123"])
    if not res_graph.stdout.strip():
        error("gen_connected_graph produced empty output.")
        
    # Permutation
    shutil.copy(TESTS_DIR / "generators/gen_perm.cpp", "gen.cpp")
    run_command(["g++", "-std=c++17", "gen.cpp", "-o", "gen"])
    
    res_perm = run_command(["./gen", "123"])
    if not res_perm.stdout.strip():
        error("gen_permutation produced empty output.")


def run_test_identical_names() -> None:
    log("Running Identical Filenames Test...")
    
    shutil.copy(TESTS_DIR / "identical_names/main.cpp", "main_same.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main_same.cpp", "-o", "main_same"])
    
    for f in (TESTS_DIR / "identical_names/input").glob("*"):
        shutil.copy(f, "input")
    for f in (TESTS_DIR / "identical_names/output").glob("*"):
        shutil.copy(f, "output")
        
    res = run_command(["./eval", "main_same", "input", "output"])
    if "OK" not in res.stdout:
        print(res.stdout)
        error("Identical filename test failed.")


def run_test_input_validator() -> None:
    log("Running Input Validator Test...")
    
    shutil.copy(TESTS_DIR / "input_validation/input_validator.cpp", ".")
    run_command(["g++", "-O2", "-std=c++17", "input_validator.cpp", "-o", "input_validator"])
    
    # Compile check_inputs if missing (should be there from setup)
    if not Path("check_inputs").exists():
        shutil.copy("../templates/check_inputs.cpp", ".")
        run_command(["g++", "-O2", "-std=c++17", "check_inputs.cpp", "-o", "check_inputs"])
        
    # Copy inputs
    for f in (TESTS_DIR / "input_validation/input").glob("*"):
        shutil.copy(f, "input")
        
    # 1. Standalone check
    res_check = run_command(["./check_inputs"])
    if "FAIL" not in res_check.stdout:
        print(res_check.stdout)
        error("check_inputs did not report failures.")
        
    # 2. Eval Integration (using main from previous test)
    # We expect ./eval to see the validator and fail on bad inputs
    res_eval = run_command(["./eval", "main", "input"])
    if "INVALID INPUT" not in res_eval.stdout:
        print(res_eval.stdout)
        error("eval did not report INVALID INPUT.")


def main() -> None:
    try:
        setup_env()
        run_test_stress_fail()
        run_test_eval_basic()
        run_test_validator()
        run_test_generators()
        run_test_identical_names()
        run_test_input_validator()
        
        # Cleanup
        os.chdir("..")
        shutil.rmtree(PROB_NAME)
        
        log("ALL TESTS PASSED.")
    except KeyboardInterrupt:
        print("\nInterrupted.")
        sys.exit(1)
    except Exception as e:
        error(f"Unexpected error: {e}")


if __name__ == "__main__":
    main()
