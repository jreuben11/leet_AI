#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
mkdir -p "$SCRIPT_DIR/out"

# Associative array mapping program names to their Makefiles
declare -A MAKEFILES

# Define Makefile for each program
MAKEFILES["1_recursion"]="Makefile.recursion"
MAKEFILES["2_linked_lists"]="Makefile.linked_lists"
MAKEFILES["skip_list"]="Makefile.skip_list"
MAKEFILES["list_search"]="Makefile.list_search"

# Ordered list of programs for menu display
PROGRAMS=("1_recursion" "2_linked_lists" "skip_list" "list_search")

# Generic compilation loop using Make
for PROGRAM in "${PROGRAMS[@]}"; do
    echo "Compiling ${PROGRAM}..."

    # Run make with the specific Makefile for this program
    make -f "${MAKEFILES[$PROGRAM]}" -C "$SCRIPT_DIR"

    if [ $? -eq 0 ]; then
        echo "${PROGRAM}: Compilation successful."
    else
        echo "${PROGRAM}: Compilation failed."
        exit 1
    fi
done

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
