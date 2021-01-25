#ifndef __CUDA_KERNELS_HH__
#define __CUDA_KERNELS_HH__

#include <cuda.h>
#include <cuda_runtime.h>

#include "KNNDataset.h"
#include "KNNClassifier.h"
#include "Common.h"

void cudaKnn1(KNNDataset *knnDataset, KNNClassifier *knnClassifier);

__global__
void cudaKnnKernel(float *trainingSamples, int *trainingClasses, float *testingSamples, int *testingClasses, void *auxVector, int numberTraining, int numberTesting, int numberFeatures, int numberClasses, int k);

__device__
float sumOfSquaredDifferencesGPU(float *sample1, float *sample2, int numberFeatures);

__device__
void doubleSortGPU(float *distances, int *indexes, int numberTraining, int k);

__device__
int findClassGPU(int *trainingClasses, int numberClasses, int k, int *indexes, int *classes);

#endif