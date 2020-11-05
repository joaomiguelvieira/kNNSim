#ifndef __CLASSIFIER_HEADER__
#define __CLASSIFIER_HEADER__

#define _BSD_SOURCE

#include <unistd.h>
#include "DistanceMetrics.h"

typedef struct KNNClassifier_ {
  // run type
  char runType[BUFLEN];
  int numberOfThreads;
  
  // distance metric
  DistanceMetric *distanceMetric;

  // number of neighbors
  int k;
  
  // profile KNN algorithm
  double distanceComputation;
  double knnFinder;
  double queryLabelFinder;
  double totalExecutionTime;

  // cuda parameters
  double cudaKernelTime;
  unsigned int cudaNumberOfBlocks;
  unsigned int cudaThreadsPerBlock;

  // hostname
  char hostname[BUFLEN];
} KNNClassifier;

KNNClassifier *newKNNClassifier(int k);
void deleteKNNClassifier(KNNClassifier *knnClassifier);

void printKNNClassifierConfiguration(KNNClassifier *knnClassifier);
void printPerformanceResults(KNNClassifier *knnClassifier);

#endif
