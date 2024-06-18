#!/bin/bash

rm -rf build
mkdir build
cd build
cmake ..
make all
find . -maxdepth 1 -type f -name "*test*" -executable | while read -r file; do "$file"; done
