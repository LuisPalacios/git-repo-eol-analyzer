#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* || "$OSTYPE" == "darwin"* ]]; then
    # Linux o macOS
    bash ./pre-commit/run-tests.sh
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    # Windows (Git Bash o similar)
    cmd.exe /c ./pre-commit/run-tests.cmd
else
    echo "Unsupported OS: $OSTYPE"
    exit 1
fi
