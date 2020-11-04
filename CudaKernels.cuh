#ifndef __CUDA_KERNELS_HH__
#define __CUDA_KERNELS_HH__

#include <cuda.h>
#include <cuda_runtime.h>

__global__
void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier);

#endif