extern "C" {
#include "CudaKernels.cuh"
}

/*
* CUDA-parallelized version of the kNN algorithm.
* 
* Each block is responsible for computing the distance of one or more
* testing samples to all training samples of the dataset. Each thread 
* within a block calculates the distance between one testing sample
* and one or more training samples.
* */
void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
	// get properties of cuda device 0
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, 0);

	// make sure that the device has enough global memory to store the
	// entire dataset
	// TODO just make sure that the training set, the results and a
	// minimal amount of testing samples fit in the gpu global memory
	unsigned long long globalMemMinSize = ((knnDataset->numberTraining + knnDataset->numberTesting) * knnDataset->numberFeatures) * sizeof(float) + (knnDataset->numberTraining + knnDataset->numberTesting) * sizeof(int);
	assert(deviceProp.totalGlobalMem > globalMemMinSize);

	// make sure that there is enough shared memory per block to store
	// the vector of distances
	unsigned long long minSharedMemPerBlock = knnDataset->numberTraining * sizeof(float);
	assert(deviceProp.sharedMemPerBlock > minSharedMemPerBlock);

	// number of threads per block is half of maximum number of threads
	// per block as multiple developers suggest that leads to the best
	// performance benefits
	unsigned int threadsPerBlock = deviceProp.maxThreadsPerBlock / 2;

	// optimal number of blocks leads to the maximum number of threads
	// per SM to be active
	unsigned int blocksPerSM = deviceProp.maxThreadsPerMultiProcessor / threadsPerBlock;
	unsigned int numberOfBlocks = deviceProp.multiProcessorCount * blocksPerSM;
	
	// the number of blocks can, however, be limited by the available
	// amount of global memory in the device
	unsigned long long remainingGlobalMemory = deviceProp.totalGlobalMem - globalMemMinSize;
	unsigned long long additionalMemoryPerBlock = knnDataset->numberTesting * sizeof(float) + knnDataset->numberTesting * sizeof(int);
	unsigned int maxNumberOfBlocks = remainingGlobalMemory / additionalMemoryPerBlock;

	if (maxNumberOfBlocks < numberOfBlocks)
		numberOfBlocks = maxNumberOfBlocks;

	printf("Number of Blocks: %u\n", numberOfBlocks);
	printf("Threads Per Block: %u\n", threadsPerBlock);

	// prepare gpu launching kernels
	assert(cudaFree(0) == cudaSuccess);

	printf("\033[1m[FATAL]:\033[0m CUDA kernels not yet implemented.\n");
	exit(-1);
}