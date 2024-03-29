// CudaFractals.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cuda_runtime_api.h>
#include <cuda.h>

// macro to catch cuda api function failures
#define DETECT_CUDA_ERROR(x) \
{   /* scope so we can reuse this variable name */ \
    cudaError_t cudaErrorCode = ##x; \
    if (cudaErrorCode != cudaSuccess) \
    { \
        std::cout << "cuda function failed on line " << __LINE__ << " of " << __FILE__ << std::endl; \
        std::cout << "error: " << cudaGetErrorString(cudaErrorCode) << std::endl; \
    } \
}

// macro to catch kernel function failures
#define DETECT_KERNEL_ERROR(x, y) \
{\
    ##x,##y; \
    if (cudaPeekAtLastError() != cudaSuccess) \
    { \
        std::cout << "kernel error: " << cudaGetErrorString(cudaGetLastError()); \
    } \
}

__global__ void dAddInts(int* a, int* b, int* sum)
{
    sum[0] = a[0] + b[0];
    a[0] = 4;
    b[0] = 7;
}

int main()
{
    int hA = 5;
    int hB = 3;
    int hSum;

    int* dA;
    int* dB;
    int* dSum;

    // allocate bytes on the gpu
    DETECT_CUDA_ERROR(cudaMalloc((void**)&dA, sizeof(int)));
    DETECT_CUDA_ERROR(cudaMalloc((void**)&dB, sizeof(int)));
    DETECT_CUDA_ERROR(cudaMalloc((void**)&dSum, sizeof(int)));

    // copy cpu bytes over to gpu
    DETECT_CUDA_ERROR(cudaMemcpy(dA, &hA, sizeof(int), cudaMemcpyHostToDevice));
    DETECT_CUDA_ERROR(cudaMemcpy(dB, &hB, sizeof(int), cudaMemcpyHostToDevice));

    // add the ints on the gpu
    DETECT_KERNEL_ERROR(dAddInts<<<1, 1>>>(dA, dB, dSum));

    // copy the memory from the gpu to the cpu
    DETECT_CUDA_ERROR(cudaMemcpy(&hSum, dSum, sizeof(int), cudaMemcpyDeviceToHost));
    
    // temp for debugging
    int tmp0;
    int tmp1;
    DETECT_CUDA_ERROR(cudaMemcpy(&tmp0, dA, sizeof(int), cudaMemcpyDeviceToHost));
    DETECT_CUDA_ERROR(cudaMemcpy(&tmp1, dB, sizeof(int), cudaMemcpyDeviceToHost));
    std::cout << tmp0 << std::endl;
    std::cout << tmp1 << std::endl;

    // free gpu memory
    DETECT_CUDA_ERROR(cudaFree(dA));
    DETECT_CUDA_ERROR(cudaFree(dB));
    DETECT_CUDA_ERROR(cudaFree(dSum));

    // print the output
    std::cout << "5 + 3 = " << hSum << std::endl;

    return 0;
}
