#ifndef __KNNALGORITHM_HEADER__
#define __KNNALGORITHM_HEADER__

#include <math.h>
#include <pthread.h>
#include "KNNDataset.h"
#include "KNNClassifier.h"
#include "Common.h"

typedef struct Arguments_ {
  int argc;
  char **argv;
} Arguments;

void knnAlgorithm(KNNDataset *knnDataset, KNNClassifier *knnClassifier);

void plainKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier, int firstSample, int lastSample);
void multithreadKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier);
void *partialKnn(void *args);

void doubleSort(KNNDataset *knnDataset, KNNClassifier *knnClassifier, float *distances, int *indexes);
int findClass(KNNDataset *knnDataset, KNNClassifier *knnClassifier, int *indexes, int *classes);

float calculateAccuracy(KNNDataset *knnDataset);
void printKNNAccuracyResults(KNNDataset *knnDataset);

#endif
