#!/bin/bash

PROGRAMS=("1_recursion")

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$SCRIPT_DIR/out"

for BASENAME in "${PROGRAMS[@]}"; do
    echo "Processing ${BASENAME}..."
    echo "Compiling..."
    gcc "$SCRIPT_DIR/${BASENAME}.c" -o "$SCRIPT_DIR/out/${BASENAME}"

    if [ $? -eq 0 ]; then
        echo -e "\n ${BASENAME}: Compilation successful. Running program:\n"
        "$SCRIPT_DIR/out/${BASENAME}"
    else
        echo -e "${BASENAME}: Compilation failed.\n"
        exit 1
    fi
done
