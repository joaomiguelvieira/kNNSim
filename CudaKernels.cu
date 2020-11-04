extern "C" {
#include "CudaKernels.cuh"
}

void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
	// get properties of cuda device 0
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, 0);

	// make sure that the device has enough global memory to store the
	// entire dataset
	// TODO just make sure that the training set, the results and a
	// minimal amount of testing samples fit in the gpu global memory
	unsigned long long globalMemMinSize = ((knnDataset->numberTraining + knnDataset->numberTesting) * (knnDataset->numberFeatures + 1)) * sizeof(float);
	assert(deviceProp.totalGlobalMem > globalMemMinSize);

	// number of threads per block is half of maximum number of threads
	// per block as multiple developers suggest that leads to the best
	// performance benefits
	unsigned int threadsPerBlock = deviceProp.maxThreadsPerBlock / 2;

	// prepare gpu launching kernels
	assert(cudaFree(0) == cudaSuccess);

	printf("\033[1m[FATAL]:\033[0m CUDA kernels not yet implemented.\n");
	exit(-1);
}