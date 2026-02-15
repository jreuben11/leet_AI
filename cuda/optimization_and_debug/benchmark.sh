#!/bin/bash

# Exit on error
set -e

echo "Building vectorization program..."
make clean
make

echo ""
echo "Running basic test..."
./out/vectorization

echo ""
echo "Running NCU benchmark..."
echo "========================================"

# Check if ncu is available
if ! command -v ncu &> /dev/null; then
    echo "Warning: ncu (NVIDIA Nsight Compute) not found in PATH"
    echo "Please install NVIDIA Nsight Compute to run benchmarks"
    exit 1
fi

# Run with NCU profiling
# Adjust the metrics as needed
ncu --set full \
    --export out/vectorization_profile \
    --force-overwrite \
    ./out/vectorization 1048576

echo ""
echo "========================================"
echo "Profile saved to: out/vectorization_profile.ncu-rep"
echo "You can view it with: ncu-ui out/vectorization_profile.ncu-rep"
