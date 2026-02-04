#!/bin/bash

g++ -O2 -std=c++17 main.cpp -o main
g++ -O2 -std=c++17 brute.cpp -o brute
g++ -O2 -std=c++17 generator.cpp -o generator
g++ -O2 -std=c++17 stress_test.cpp -o stress_test
if [ -f eval.cpp ]; then
    g++ -O2 -std=c++17 eval.cpp -o eval
fi
if [ -f validator.cpp ]; then
    g++ -O2 -std=c++17 validator.cpp -o validator
fi
if [ -f input_validator.cpp ]; then
    g++ -O2 -std=c++17 input_validator.cpp -o input_validator
fi
if [ -f check_inputs.cpp ]; then
    g++ -O2 -std=c++17 check_inputs.cpp -o check_inputs
fi
echo "Compilation finished"
