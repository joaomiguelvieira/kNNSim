extern "C" {
#include "CudaKernels.cuh"
}

__global__
void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
	printf("\033[1m[ERROR]:\033[0m CUDA kernels not yet implemented.\n");
}