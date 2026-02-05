#!/bin/bash

TEMPLATE_DIR="$(pwd)/templates"

echo "Compiling problem_setup with templates from: $TEMPLATE_DIR"

g++ -std=c++17 -O2 -DTEMPLATE_DIR="\"$TEMPLATE_DIR\"" src/problem_setup.cpp -o problem_setup

echo "Built problem_setup. Run ./problem_setup <problem_name> to create a new problem."
