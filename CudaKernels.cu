extern "C" {
#include "CudaKernels.cuh"
}

/*
 * CUDA-parallelized version of the kNN algorithm (1).
 *
 * Each block is responsible for computing the distance of one or more
 * testing samples to all training samples of the dataset. Each thread
 * within a block calculates the distance between one testing sample
 * and one or more training samples.
 * */
void cudaKnn1(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
	// get properties of cuda device 0
	cudaDeviceProp deviceProp;
	cudaGetDeviceProperties(&deviceProp, 0);

	// make sure that the device has enough global memory to store the
	// entire dataset
	unsigned long long globalMemMinSize = ((knnDataset->numberTraining + knnDataset->numberTesting) * knnDataset->numberFeatures) * sizeof(float) + (knnDataset->numberTraining + knnDataset->numberTesting) * sizeof(int);
	assert(deviceProp.totalGlobalMem > globalMemMinSize);

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
	unsigned long long additionalMemoryPerBlock = knnDataset->numberTraining * sizeof(float) + knnDataset->numberTraining * sizeof(int);
	unsigned int maxNumberOfBlocks = remainingGlobalMemory / additionalMemoryPerBlock;

	if (maxNumberOfBlocks < numberOfBlocks)
		numberOfBlocks = maxNumberOfBlocks;

	// make sure that there is enough remaining global memory to launch
	// at least one block
	assert(numberOfBlocks > 0);

	// assign the calculated properties to the classifier
	strcpy(knnClassifier->cudaDeviceName, deviceProp.name);
	knnClassifier->cudaPeakGlobalMemory = globalMemMinSize + numberOfBlocks * additionalMemoryPerBlock;
	knnClassifier->cudaNumberOfBlocks = numberOfBlocks;
	knnClassifier->cudaThreadsPerBlock = threadsPerBlock;
	knnClassifier->cudaDeviceUtilization = numberOfBlocks * threadsPerBlock / (deviceProp.maxThreadsPerMultiProcessor * deviceProp.multiProcessorCount);

	// allocate memory in the device
	float *trainingSamplesGPU, *testingSamplesGPU;
	int *trainingClassesGPU, *testingClassesGPU;
	void *auxVectorGPU;

	// allocate operands
	assert(cudaMalloc((void **) &trainingSamplesGPU, knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
	assert(cudaMalloc((void **) &testingSamplesGPU,  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
	assert(cudaMalloc((void **) &trainingClassesGPU, knnDataset->numberTraining                              * sizeof(int))   == cudaSuccess);

	// allocate result vector
	assert(cudaMalloc((void **) &testingClassesGPU, knnDataset->numberTesting * sizeof(int)) == cudaSuccess);

	// allocate auxiliary vector
	assert(cudaMalloc((void **) &auxVectorGPU, knnDataset->numberTraining * numberOfBlocks * 2 * sizeof(float)) == cudaSuccess);

	// copy operands to the device
	assert(cudaMemcpy(trainingSamplesGPU, knnDataset->trainingSamples[0], knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
	assert(cudaMemcpy(testingSamplesGPU,  knnDataset->testingSamples[0],  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
	assert(cudaMemcpy(trainingClassesGPU, knnDataset->trainingClasses,    knnDataset->numberTraining                              * sizeof(int),   cudaMemcpyHostToDevice) == cudaSuccess);

	// measure cuda kernel time
	cudaEvent_t cudaKernelStart, cudaKernelStop;
	assert(cudaEventCreate(&cudaKernelStart) == cudaSuccess);
	assert(cudaEventCreate(&cudaKernelStop) == cudaSuccess);

	// launch cuda kernel
	assert(cudaEventRecord(cudaKernelStart) == cudaSuccess);
	cudaKnnKernel1<<<numberOfBlocks, threadsPerBlock>>>(trainingSamplesGPU, trainingClassesGPU, testingSamplesGPU, testingClassesGPU, auxVectorGPU, knnDataset->numberTraining, knnDataset->numberTesting, knnDataset->numberFeatures, knnDataset->numberClasses, knnClassifier->k);
	assert(cudaEventRecord(cudaKernelStop) == cudaSuccess);

	// assign cuda kernel time to the classifier
	assert(cudaEventSynchronize(cudaKernelStop) == cudaSuccess);
	float cudaElapsedMs;
	assert(cudaEventElapsedTime(&cudaElapsedMs, cudaKernelStart, cudaKernelStop) == cudaSuccess);
	knnClassifier->cudaKernelTime = cudaElapsedMs / 1000;

	// check if any errors have occured
	assert(cudaGetLastError() == cudaSuccess);

	// retrieve results back to host
	assert(cudaMemcpy(knnDataset->testingClasses, testingClassesGPU, knnDataset->numberTesting * sizeof(int), cudaMemcpyDeviceToHost) == cudaSuccess);

	// cleanup
	assert(cudaFree(auxVectorGPU) == cudaSuccess);
	assert(cudaFree(testingClassesGPU) == cudaSuccess);
	assert(cudaFree(trainingClassesGPU) == cudaSuccess);
	assert(cudaFree(testingSamplesGPU) == cudaSuccess);
	assert(cudaFree(trainingSamplesGPU) == cudaSuccess);
}

__global__
void cudaKnnKernel1(float *trainingSamples, int *trainingClasses, float *testingSamples, int *testingClasses, void *auxVector, int numberTraining, int numberTesting, int numberFeatures, int numberClasses, int k) {
	// calculate the indexes of the auxiliary arrays
	float *auxDistances = ((float *) auxVector) + (blockIdx.x * 2 * numberTraining);
	int *auxIndexes = (int *) (((int *) auxVector) + ((blockIdx.x * 2 + 1) * numberTraining));

	// each block processes the testing samples whose indexes are a
	// multiple of the block index
	for (int i = blockIdx.x; i < numberTesting; i += gridDim.x) {
		// calculate address of testing sample
		float *testingSample = testingSamples + i * numberFeatures;

		// each thread processes the training samples whose indexes
		// are a multiple of the thread index
		for (int j = threadIdx.x; j < numberTraining; j += blockDim.x) {
			// calculate distance of training sample
			float *trainingSample = trainingSamples + j * numberFeatures;

			// calculate distance and initialize distance index array
			auxDistances[j] = sumOfSquaredDifferencesGPU(testingSample, trainingSample, numberFeatures);
			auxIndexes[j] = j;
		}

		// sync threads
		__syncthreads();

		// last two phases of knn are sequential
		if (threadIdx.x == 0) {
			// thread 0 double sorts distance and index arrays
			doubleSortGPU(auxDistances, auxIndexes, numberTraining, k);

			// thread 0 does class assignement
			testingClasses[i] = findClassGPU(trainingClasses, numberClasses, k, auxIndexes, (int *) auxDistances);
		}

		// sync threads
		__syncthreads();
	}
}

__device__
float sumOfSquaredDifferencesGPU(float *sample1, float *sample2, int numberFeatures) {
  float difference, distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    difference = sample1[i] - sample2[i];
    distance += difference * difference;
  }

  return distance;
}

__device__
void doubleSortGPU(float *distances, int *indexes, int numberTraining, int k) {
  int minimum, aux;

  // sort the minimum k elements
  for (int i = 0; i < k; i++) {
    minimum = i;

    // find the next minimum value
    for (int j = i + 1; j < numberTraining; j++)
      if (distances[j] < distances[minimum])
        minimum = j;

    // exchange values
    aux = distances[i];
    distances[i] = distances[minimum];
    distances[minimum] = aux;

    // exchange indexes
    aux = indexes[i];
    indexes[i] = indexes[minimum];
    indexes[minimum] = aux;
  }
}

__device__
int findClassGPU(int *trainingClasses, int numberClasses, int k, int *indexes, int *classes) {
  int maximum = 0;

  // initialize vote array
  for (int i = 0; i < numberClasses; i++)
    classes[i] = 0;

  // voting process
  for (int i = 0; i < k; i++)
    classes[trainingClasses[indexes[i]]]++;

  // find the most voted class
  for (int i = 1; i < numberClasses; i++)
    if (classes[i] > classes[maximum])
      maximum = i;

  return maximum;
}

/*
 * CUDA parallelized version of the kNN algorithm (2).
 *
 * Each block is responsible for computing the distance of one or more
 * testing samples to all training samples of the dataset. Each thread
 * within a block calculates the distance between one testing sample
 * and one or more training samples. Then, it updates its own list of
 * the k closest samples of the training set and keeps it on a shared
 * vector. Finally, the thread with the index 0 sorts the vector and
 * determines the closest k samples.
 * */
void cudaKnn2(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
    // get properties of cuda device 0
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp, 0);

    // make sure that the device has enough global memory to store the
    // entire dataset
    unsigned long long globalMemMinSize = ((knnDataset->numberTraining + knnDataset->numberTesting) * knnDataset->numberFeatures) * sizeof(float) + (knnDataset->numberTraining + knnDataset->numberTesting) * sizeof(int);
    assert(deviceProp.totalGlobalMem > globalMemMinSize);

    // number of threads per block is half of maximum number of threads
    // per block as multiple developers suggest that leads to the best
    // performance benefits
    unsigned int threadsPerBlock = deviceProp.maxThreadsPerBlock / 2;

    // optimal number of blocks leads to the maximum number of threads
    // per SM to be active
    unsigned int blocksPerSM = deviceProp.maxThreadsPerMultiProcessor / threadsPerBlock;
    unsigned int numberOfBlocks = deviceProp.multiProcessorCount * blocksPerSM;

    // the number of blocks can, however, be limited by the available
    // amount of shared memory in the device
    unsigned long long requiredSharedMemoryPerThread = knnClassifier->k * (sizeof(float) + sizeof(int));
    unsigned long long requiredSharedMemoryPerBlock = requiredSharedMemoryPerThread * threadsPerBlock;
    assert(requiredSharedMemoryPerBlock <= deviceProp.sharedMemPerBlock);

    // assign the calculated properties to the classifier
    strcpy(knnClassifier->cudaDeviceName, deviceProp.name);
    knnClassifier->cudaPeakGlobalMemory = globalMemMinSize;
    knnClassifier->cudaNumberOfBlocks = numberOfBlocks;
    knnClassifier->cudaThreadsPerBlock = threadsPerBlock;
    knnClassifier->cudaDeviceUtilization = numberOfBlocks * threadsPerBlock / (deviceProp.maxThreadsPerMultiProcessor * deviceProp.multiProcessorCount);

    // allocate memory in the device
    float *trainingSamplesGPU, *testingSamplesGPU;
    int *trainingClassesGPU, *testingClassesGPU;

    // allocate operands
    assert(cudaMalloc((void **) &trainingSamplesGPU, knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
    assert(cudaMalloc((void **) &testingSamplesGPU,  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float)) == cudaSuccess);
    assert(cudaMalloc((void **) &trainingClassesGPU, knnDataset->numberTraining                              * sizeof(int))   == cudaSuccess);

    // allocate result vector
    assert(cudaMalloc((void **) &testingClassesGPU, knnDataset->numberTesting * sizeof(int)) == cudaSuccess);

    // copy operands to the device
    assert(cudaMemcpy(trainingSamplesGPU, knnDataset->trainingSamples[0], knnDataset->numberTraining * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
    assert(cudaMemcpy(testingSamplesGPU,  knnDataset->testingSamples[0],  knnDataset->numberTesting  * knnDataset->numberFeatures * sizeof(float), cudaMemcpyHostToDevice) == cudaSuccess);
    assert(cudaMemcpy(trainingClassesGPU, knnDataset->trainingClasses,    knnDataset->numberTraining                              * sizeof(int),   cudaMemcpyHostToDevice) == cudaSuccess);

    // measure cuda kernel time
    cudaEvent_t cudaKernelStart, cudaKernelStop;
    assert(cudaEventCreate(&cudaKernelStart) == cudaSuccess);
    assert(cudaEventCreate(&cudaKernelStop) == cudaSuccess);

    exit(-1);

    // launch cuda kernel
    assert(cudaEventRecord(cudaKernelStart) == cudaSuccess);
    cudaKnnKernel2<<<numberOfBlocks, threadsPerBlock>>>(trainingSamplesGPU, trainingClassesGPU, testingSamplesGPU, testingClassesGPU, knnDataset->numberTraining, knnDataset->numberTesting, knnDataset->numberFeatures, knnDataset->numberClasses, knnClassifier->k);
    assert(cudaEventRecord(cudaKernelStop) == cudaSuccess);

    // assign cuda kernel time to the classifier
    assert(cudaEventSynchronize(cudaKernelStop) == cudaSuccess);
    float cudaElapsedMs;
    assert(cudaEventElapsedTime(&cudaElapsedMs, cudaKernelStart, cudaKernelStop) == cudaSuccess);
    knnClassifier->cudaKernelTime = cudaElapsedMs / 1000;

    // check if any errors have occurred
    assert(cudaGetLastError() == cudaSuccess);

    // retrieve results back to host
    assert(cudaMemcpy(knnDataset->testingClasses, testingClassesGPU, knnDataset->numberTesting * sizeof(int), cudaMemcpyDeviceToHost) == cudaSuccess);

    // cleanup
    assert(cudaFree(testingClassesGPU) == cudaSuccess);
    assert(cudaFree(trainingClassesGPU) == cudaSuccess);
    assert(cudaFree(testingSamplesGPU) == cudaSuccess);
    assert(cudaFree(trainingSamplesGPU) == cudaSuccess);
}

__global__
void cudaKnnKernel2(float *trainingSamples, int *trainingClasses, float *testingSamples, int *testingClasses, int numberTraining, int numberTesting, int numberFeatures, int numberClasses, int k) {

}