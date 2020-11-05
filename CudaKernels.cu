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
	// and transfer more testing samples at the rithm that the gpu
	// processes the previous ones
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

	// assign the calculated properties to the classifier
	knnClassifier->cudaNumberOfBlocks = numberOfBlocks;
	knnClassifier->cudaThreadsPerBlock = threadsPerBlock;

	// allocate memory in the device
	float *trainingSamplesGPU, *testingSamplesGPU;
	int *trainingClassesGPU, *testingClassesGPU;
	void *auxVectorGPU;

	// allocate operands
	assert(cudaMalloc((void **) &trainingSamplesGPU, knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
	assert(cudaMalloc((void **) &testingSamplesGPU,  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
	assert(cudaMalloc((void **) &trainingClassesGPU, knnDataset->numberTraining                              * sizeof(int))   == cudaSuccess);

	// alloocate result vector
	assert(cudaMalloc((void **) &testingClassesGPU, knnDataset->numberTesting * sizeof(int)) == cudaSuccess);

	// allocate auxiliary vector
	assert(cudaMalloc((void **) &auxVectorGPU, knnDataset->numberTraining * numberOfBlocks * 2 * sizeof(float)) == cudaSuccess);

	// copy operands to the device
	assert(cudaMemcpy(trainingSamplesGPU, knnDataset->trainingSamples[0], knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
	assert(cudaMemcpy(testingSamplesGPU,  knnDataset->testingSamples[0],  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
	assert(cudaMemcpy(trainingClassesGPU, knnDataset->trainingClasses,    knnDataset->numberTraining                              * sizeof(int),   cudaMemcpyHostToDevice) == cudaSuccess);

	// launch cuda kernel
	cudaKnnKernel<<<numberOfBlocks, threadsPerBlock>>>(trainingSamplesGPU, trainingClassesGPU, testingSamplesGPU, testingClassesGPU, auxVectorGPU, knnDataset->numberTraining, knnDataset->numberTesting, knnDataset->numberFeatures, knnDataset->numberClasses);
	assert(cudaGetLastError() == cudaSuccess);

	// retrieve results back to host
	assert(cudaMemcpy(knnDataset->testingClasses, testingClassesGPU, knnDataset->numberTesting * sizeof(int), cudaMemcpyDeviceToHost) == cudaSuccess);

	// free device memory
	assert(cudaFree(auxVectorGPU) == cudaSuccess);
	assert(cudaFree(testingClassesGPU) == cudaSuccess);
	assert(cudaFree(trainingClassesGPU) == cudaSuccess);
	assert(cudaFree(testingSamplesGPU) == cudaSuccess);
	assert(cudaFree(trainingSamplesGPU) == cudaSuccess);

	for (int i = 0; i < knnDataset->numberTesting; i++)
		printf("%d ", knnDataset->testingClasses[i]);

	printf("\n");

	printf("\033[1m[FATAL]:\033[0m CUDA kernels not yet fully implemented.\n");
	exit(-1);
}

__global__
void cudaKnnKernel(float *trainingSamples, int *trainingClasses, float *testingSamples, int *testingClasses, void *auxVector, int numberTraining, int numberTesting, int numberFeatures, int numberClasses) {
	// calculate the indexes of the auxiliary arrays
	float *auxDistances = ((float *) auxVector) + (blockIdx.x * 2 * numberTraining);
	int *auxIndexes = (int *) (((int *) auxVector) + ((blockIdx.x * 2 + 1) * numberTraining));

	// each block processes the testing samples whose indexes are a
	// multiple of the block index
	for (int i = blockIdx.x; i < numberTesting; i += gridDim.x) {
		// each thread processes the training samples whose indexes
		// are a multiple of the thread index
		for (int j = threadIdx.x; j < numberTraining; j += blockDim.x) {
			// calculate distance and initialize distance index array
			auxDistances[j] = 0;
			auxIndexes[j] = j;
		}

		// sync threads
		__syncthreads();

		// thread 0 double sorts distance and index arrays

		// thread 0 does class assignement
		testingClasses[i] = -2;
	}
}