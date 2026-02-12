#!/bin/bash

# Compiler
CXX=g++

CXXFLAGS="-std=c++20 -Wall -Wextra -Ihpp_file"

# Output binary name
OUTPUT="syxc"

# Source files
START="src/sys_start.cpp"
CORE="src/core/lexer.cpp src/core/parser.cpp src/core/codegen.cpp"
MODULE="src/modules/driver.cpp"
SRC="$START $CORE $MODULE"

# Clean previous build
echo "Cleaning old build..."
rm -f "$OUTPUT"

# Compile
echo "Building project..."
$CXX $CXXFLAGS $SRC -o "$OUTPUT"

# Check for success
if [ $? -eq 0 ]; then
    echo "Build successful!"
    echo "Run with: ./$OUTPUT"
else
    echo "Build failed."
fi


