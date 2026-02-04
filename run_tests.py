#!/usr/bin/env python3

import os
import sys
import shutil
import subprocess
from pathlib import Path


PROBLEM_NAME = "TestSuiteProblem"
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
        sys.exit(1)


def setup_env() -> None:
    log("Setting up fresh environment...")
    
    if Path("install.sh").exists():
        pass
    elif Path("../install.sh").exists():
        os.chdir("..")
    
    root_path = Path.cwd()
    prob_path = root_path / PROBLEM_NAME
    
    if prob_path.exists():
        shutil.rmtree(prob_path)
    
    cp_eval_bin = root_path / "cp-eval"
    if not cp_eval_bin.exists():
        error("cp-eval binary not found. Run ./install.sh first.")
    
    run_command([str(cp_eval_bin), PROBLEM_NAME], capture=True)
    
    if not prob_path.exists():
        error("Problem directory not created.")
    
    os.chdir(prob_path)
    
    if not Path("compile.sh").exists():
        error("compile.sh not found in problem directory.")
        
    res = run_command(["./compile.sh"])
    if res.returncode != 0:
        error("Tools compilation failed.")


def run_test_stress_fail() -> None:
    log("Running Stress Test (Expected Failure)...")
    
    shutil.copy(TESTS_DIR / "stress_fail/main.cpp", ".")
    shutil.copy(TESTS_DIR / "stress_fail/brute.cpp", ".")
    
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    run_command(["g++", "-O2", "-std=c++17", "brute.cpp", "-o", "brute"])
    
    res = run_command(["./stress_test", "generator", "main", "brute"])
    
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
    
    shutil.copy(TESTS_DIR / "validator/main_fail.cpp", "main.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    
    res_fail = run_command(["./eval", "main", "input"])
    if "WA (Validator)" not in res_fail.stdout:
        print(res_fail.stdout)
        error("Auto-validator did not detect mismatch.")
        
    shutil.copy(TESTS_DIR / "validator/main_pass.cpp", "main.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    
    res_pass = run_command(["./eval", "main", "input"])
    if "OK" not in res_pass.stdout:
        print(res_pass.stdout)
        error("Auto-validator did not report OK on match.")


def run_test_generators() -> None:
    log("Running Generator Tests...")
    
    shutil.copy(TESTS_DIR / "generators/gen_connected_graph.cpp", "generator.cpp")
    run_command(["g++", "-std=c++17", "generator.cpp", "-o", "generator"])
    
    res_graph = run_command(["./generator", "123"])
    if not res_graph.stdout.strip():
        error("genConnectedGraph produced empty output.")
        
    shutil.copy(TESTS_DIR / "generators/gen_general_graph.cpp", "generator.cpp")
    run_command(["g++", "-std=c++17", "generator.cpp", "-o", "generator"])
    
    res_gen_graph = run_command(["./generator", "123"])
    if not res_gen_graph.stdout.strip():
        error("genGraph produced empty output.")

    shutil.copy(TESTS_DIR / "generators/gen_tree.cpp", "generator.cpp")
    run_command(["g++", "-std=c++17", "generator.cpp", "-o", "generator"])
    
    res_tree = run_command(["./generator", "123"])
    if not res_tree.stdout.strip():
        error("genTree produced empty output.")

    shutil.copy(TESTS_DIR / "generators/gen_array.cpp", "generator.cpp")
    run_command(["g++", "-std=c++17", "generator.cpp", "-o", "generator"])
    
    res_array = run_command(["./generator", "123"])
    if not res_array.stdout.strip():
        error("genArray produced empty output.")
        
    shutil.copy(TESTS_DIR / "generators/gen_perm.cpp", "generator.cpp")
    run_command(["g++", "-std=c++17", "generator.cpp", "-o", "generator"])
    
    res_perm = run_command(["./generator", "123"])
    if not res_perm.stdout.strip():
        error("genPermutation produced empty output.")


def run_test_match_names() -> None:
    log("Running Matching Names Test...")
    
    shutil.copy(TESTS_DIR / "match_names/main.cpp", "main_match.cpp")
    run_command(["g++", "-O2", "-std=c++17", "main_match.cpp", "-o", "main_match"])
    
    for f in (TESTS_DIR / "match_names/input").glob("*"):
        shutil.copy(f, "input")
    for f in (TESTS_DIR / "match_names/output").glob("*"):
        shutil.copy(f, "output")
        
    res = run_command(["./eval", "main_match", "input", "output"])
    if "OK" not in res.stdout:
        print(res.stdout)
        error("Match names test failed.")


def run_test_input_validator() -> None:
    log("Running Input Validator Test...")
    
    shutil.copy(TESTS_DIR / "input_validation/input_validator.cpp", ".")
    run_command(["g++", "-O2", "-std=c++17", "input_validator.cpp", "-o", "input_validator"])
    
    if not Path("check_inputs").exists():
        shutil.copy("../templates/check_inputs.cpp", ".")
        run_command(["g++", "-O2", "-std=c++17", "check_inputs.cpp", "-o", "check_inputs"])
        
    for f in (TESTS_DIR / "input_validation/input").glob("*"):
        shutil.copy(f, "input")
        
    res_check = run_command(["./check_inputs"])
    if "FAIL" not in res_check.stdout:
        print(res_check.stdout)
        error("check_inputs did not report failures.")
        
    res_eval = run_command(["./eval", "main", "input"])
    if "INVALID INPUT" not in res_eval.stdout:
        print(res_eval.stdout)
        error("eval did not report INVALID INPUT.")


def run_test_stress_validation() -> None:
    log("Running Stress Validation Test...")
    
    shutil.copy(TESTS_DIR / "stress_validation/gen.cpp", "generator.cpp")
    shutil.copy(TESTS_DIR / "stress_validation/input_validator.cpp", "input_validator.cpp")
    shutil.copy(TESTS_DIR / "stress_validation/main.cpp", "main.cpp")
    shutil.copy(TESTS_DIR / "stress_validation/brute.cpp", "brute.cpp")
    
    run_command(["g++", "-O2", "-std=c++17", "generator.cpp", "-o", "generator"])
    run_command(["g++", "-O2", "-std=c++17", "input_validator.cpp", "-o", "input_validator"])
    run_command(["g++", "-O2", "-std=c++17", "main.cpp", "-o", "main"])
    run_command(["g++", "-O2", "-std=c++17", "brute.cpp", "-o", "brute"])
    
    res = run_command(["./stress_test", "generator", "main", "brute"])
    
    if res.returncode == 0:
        error("Stress test did not fail on invalid input.")
        
    if "Generated input failed validation" not in res.stderr:
        print(res.stderr)
        error("Stress test did not report validation failure.")


def main() -> None:
    try:
        setup_env()
        run_test_stress_fail()
        run_test_eval_basic()
        run_test_validator()
        run_test_generators()
        run_test_generators()
        run_test_match_names()
        run_test_input_validator()
        run_test_input_validator()
        run_test_stress_validation()
        
        os.chdir("..")
        shutil.rmtree(PROBLEM_NAME)
        
        log("ALL TESTS PASSED.")
    except KeyboardInterrupt:
        print("\nInterrupted.")
        sys.exit(1)
    except Exception as e:
        error(f"Unexpected error: {e}")


if __name__ == "__main__":
    main()
