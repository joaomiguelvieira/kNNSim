extern "C" {
#include "CudaKernels.cuh"
}

void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
	// prepare gpu launching kernels
	assert(cudaFree(0) == cudaSuccess);

	printf("\033[1m[FATAL]:\033[0m CUDA kernels not yet implemented.\n");
	exit(-1);
}