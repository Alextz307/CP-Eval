#!/bin/bash
TEMPLATE_DIR="$(pwd)/templates"

echo "Compiling cp-eval with templates from: $TEMPLATE_DIR"

g++ -std=c++17 -O2 -DTEMPLATE_DIR="\"$TEMPLATE_DIR\"" src/cp_setup.cpp -o cp-eval

echo "Built cp-eval. Run ./cp-eval <problem_name> to create a new problem."
