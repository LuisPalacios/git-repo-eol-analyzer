#!/bin/bash
# ./scripts/clang-doc/generate_clang_doc.sh
# Generate clang-doc documentation

# Directories and files
SRC_DIR="./src"
BUILD_DIR="./build"
OUTPUT_DIR="./doc/clang-doc"
SRC_DIR="./src"

# Ensure compile_commands.json exists
if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
  echo "Error: compile_commands.json not found in $BUILD_DIR. Please compile your project first."
  exit 1
fi

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Run clang-doc
clang-doc -p="$BUILD_DIR" --output="$OUTPUT_DIR" --format=html $SRC_DIR
#clang-doc -p out/tmp --output=doc/clang-doc --format=html --source-root=src

# Confirm success
if [ $? -eq 0 ]; then
  echo "Clang-doc documentation successfully generated in $OUTPUT_DIR."
else
  echo "Error: Failed to generate clang-doc documentation."
  exit 1
fi
