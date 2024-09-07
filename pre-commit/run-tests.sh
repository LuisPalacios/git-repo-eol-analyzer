#!/bin/bash

# Navigate to the root of the repository
REPO_ROOT=$(git rev-parse --show-toplevel)
cd "$REPO_ROOT"

# Build the project
echo "Building the project..."
cmake -S . -B build
cmake --build build

# Run the tests
echo "Running tests..."
cd build
ctest --output-on-failure
TEST_RESULT=$?

# If the tests fail, abort the commit
if [ $TEST_RESULT -ne 0 ]; then
  echo "Tests failed. Aborting commit."
  exit 1
fi

echo "Tests passed. Proceeding with commit."
exit 0