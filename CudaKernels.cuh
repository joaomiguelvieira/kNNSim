#ifndef __CUDA_KERNELS_HH__
#define __CUDA_KERNELS_HH__

#include <cuda.h>
#include <cuda_runtime.h>

#include "KNNDataset.h"
#include "KNNClassifier.h"
#include "Common.h"

void cudaKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier);

__global__
void cudaKnnKernel(float *trainingSamples, float *trainingClasses, float *testingSamples, float *testingClasses, float *auxVector, int numberTraining, int numberTesting, int numberFeatures, int numberClasses);

#endif