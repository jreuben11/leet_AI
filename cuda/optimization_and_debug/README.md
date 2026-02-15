# CUDA Vectorization - Optimization and Profiling

This project demonstrates basic CUDA vector operations with profiling capabilities using NVIDIA Nsight Compute (NCU).

## Prerequisites

- NVIDIA GPU with CUDA support
- CUDA Toolkit (13.0+)
- NVIDIA Nsight Compute (ncu) for profiling

## GPU Performance Counter Permissions

To use NCU profiling tools, you need to grant permission to access NVIDIA GPU performance counters.

### Why is this needed?

By default, NVIDIA restricts access to GPU performance counters to admin users only for security reasons. To run `ncu` without sudo, you need to change this setting.

### Setup (one-time configuration)

1. Create the modprobe configuration file:
```bash
sudo cp nvidia-profiling.conf /etc/modprobe.d/
```

2. Apply the changes (choose one):

   **Option A: Reboot** (simplest)
   ```bash
   sudo reboot
   ```

   **Option B: Reload NVIDIA driver** (no reboot)
   ```bash
   sudo modprobe -r nvidia_uvm nvidia_drm nvidia_modeset nvidia
   sudo modprobe nvidia
   ```

3. Verify the setting:
```bash
cat /proc/driver/nvidia/params | grep RmProfilingAdminOnly
# Should output: RmProfilingAdminOnly: 0
```

### What does the config do?

The `nvidia-profiling.conf` file sets the kernel module parameter:
```
options nvidia NVreg_RestrictProfilingToAdminUsers=0
```

This tells the NVIDIA driver to allow non-root users to access GPU performance counters, enabling tools like `ncu`, `nvprof`, and Nsight Systems to collect detailed metrics.

## Compilation

### Build the program:
```bash
make
```

This compiles `vectorization.cu` with:
- **Optimization**: `-O3` for maximum performance
- **Architecture**: `-arch=sm_89` (RTX 4090)
- **C++ Standard**: `-std=c++20` (modern C++ features)
- **Output**: `out/vectorization`

### Other make targets:
```bash
make clean    # Remove build artifacts
make run      # Build and run the program
```

## Running the Program

### Basic execution:
```bash
./out/vectorization           # Default: 1M elements
./out/vectorization 2097152   # Custom: 2M elements
```

### Profile with NCU:
```bash
./benchmark.sh
```

This script will:
1. Clean and rebuild the program
2. Run a basic functionality test
3. Profile with NCU using full metric collection
4. Save results to `out/vectorization_profile.ncu-rep`

### Manual profiling:
```bash
# Basic profiling
ncu ./out/vectorization

# Full metrics with export
ncu --set full --export out/profile --force-overwrite ./out/vectorization

# View saved profile
ncu-ui out/profile.ncu-rep
```

## Understanding the Code

The `vec_basic` kernel performs a simple vector initialization:
- Each thread computes its global ID: `blockIdx.x * blockDim.x + threadIdx.x`
- Sets one vector element to 0.0
- Demonstrates basic CUDA memory access patterns

### Launch Configuration:
- **Block size**: 256 threads per block
- **Grid size**: Calculated to cover all elements: `(n + blockSize - 1) / blockSize`

## Profiling Metrics to Watch

When analyzing NCU results, pay attention to:
- **Memory throughput**: How efficiently global memory is accessed
- **Occupancy**: Percentage of active warps vs. theoretical maximum
- **Memory transactions**: Coalesced vs. uncoalesced accesses
- **Kernel duration**: Actual execution time

## Next Steps

This basic example can be extended to explore:
- Vectorized loads (float4, etc.)
- Shared memory usage
- Memory coalescing patterns
- Different block/grid configurations
