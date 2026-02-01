#!/bin/bash

PROGRAMS=("1_basics")

mkdir -p out

for BASENAME in "${PROGRAMS[@]}"; do
    echo "Processing ${BASENAME}..."
    echo "Compiling..."
    gcc "${BASENAME}.c" -o "out/${BASENAME}"

    if [ $? -eq 0 ]; then
        echo -e "\n ${BASENAME}: Compilation successful. Running program:\n"
        ./out/"${BASENAME}"
    else
        echo -e "${BASENAME}: Compilation failed.\n"
        exit 1
    fi
done
