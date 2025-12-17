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
OUTPUT="sysc"

# Source files
SRC="main.cpp cpp_file/lexer.cpp cpp_file/parser.cpp cpp_file/codegen.cpp"

# Clean previous build
echo "🧹 Cleaning old build..."
rm -f "$OUTPUT"

# Compile
echo "🔧 Building project..."
$CXX $CXXFLAGS $SRC -o "$OUTPUT"

# Check for success
if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "Run with: ./sysc"
else
    echo "❌ Build failed."
fi
