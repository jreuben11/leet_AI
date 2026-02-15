#include <cuda_runtime.h>
#include <stdio.h>

__global__ void vec_basic(float* v, int n)
{
    // Compute offset
    int id = blockIdx.x * blockDim.x + threadIdx.x;
    if (id < n) { // Safety
        v[id] = 0.0; // Clear one vector element..
    }
}

int main(int argc, char** argv)
{
    int n = 1024 * 1024; // 1M elements
    if (argc > 1) {
        n = atoi(argv[1]);
    }

    size_t bytes = n * sizeof(float);

    // Allocate device memory
    float* d_v;
    cudaMalloc(&d_v, bytes);

    // Launch configuration
    int blockSize = 256;
    int gridSize = (n + blockSize - 1) / blockSize;

    // Launch kernel
    vec_basic<<<gridSize, blockSize>>>(d_v, n);

    // Wait for completion
    cudaDeviceSynchronize();

    // Check for errors
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(err));
        return 1;
    }

    // Cleanup
    cudaFree(d_v);

    printf("Successfully cleared %d vector elements\n", n);
    return 0;
}