#!/bin/bash
# ==========================================
# Simple build script for your compiler project
# Using g++ (GNU C++ Compiler)
# ==========================================

# Compiler
CXX=g++

# Compiler flags:
# -std=c++17 : use C++17 standard
# -Wall -Wextra : show all warnings
# -Ihpp_file : add header include directory
CXXFLAGS=" -Wall -Wextra -Ihpp_file"

# Output binary name
OUTPUT="syxc"

# Source files
START="src/sys_start.cpp"
CORE="src/core/lexer.cpp src/core/parser.cpp src/core/codegen.cpp" 
SRC="$START $CORE"

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
