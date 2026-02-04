#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# List of programs for menu display
PROGRAMS=("1_recursion" "2_linked_lists" "3_stacks_and_queues" "skip_list" "list_search" "dynamic_programming")

# Compile all programs using the master Makefile
echo "Building all programs..."
make -C "$SCRIPT_DIR"

if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

echo "Build successful."

# Interactive menu to run programs
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
