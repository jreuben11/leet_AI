#!/bin/bash

PROGRAMS=("1_recursion" "2_linked_lists" "skip_list")

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$SCRIPT_DIR/out"

for BASENAME in "${PROGRAMS[@]}"; do
    echo "Compiling ${BASENAME}..."
    gcc "$SCRIPT_DIR/${BASENAME}.c" -o "$SCRIPT_DIR/out/${BASENAME}"
    
    if [ $? -eq 0 ]; then
        echo "${BASENAME}: Compilation successful."
    else
        echo "${BASENAME}: Compilation failed."
        exit 1
    fi
done

while true; do
    echo -e "\nSelect a program to run:"
    for i in "${!PROGRAMS[@]}"; do
        echo "$((i+1)). ${PROGRAMS[$i]}"
    done
    echo "x. Exit"
    
    read -p "Enter choice: " choice
    
    if [ "$choice" == "x" ]; then
        break
    fi
    
    if [[ "$choice" =~ ^[0-9]+$ ]] && [ "$choice" -ge 1 ] && [ "$choice" -le "${#PROGRAMS[@]}" ]; then
        PROGRAM_INDEX=$((choice-1))
        SELECTED_PROGRAM="${PROGRAMS[$PROGRAM_INDEX]}"
        echo -e "\nRunning ${SELECTED_PROGRAM}...\n"
        "$SCRIPT_DIR/out/${SELECTED_PROGRAM}"
    else
        echo "Invalid choice."
    fi
done
