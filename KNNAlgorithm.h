#ifndef __KNNALGORITHM_HEADER__
#define __KNNALGORITHM_HEADER__

/*
 * libraries includes go here
 */
#include <pthread.h>
#include "Dataset.h"

 /*
  * structures are declared here
  */
typedef enum RunType_ {plain, multithread} RunType;
typedef struct Arguments_ {
  int argc;
  char **argv;
} Arguments;

/*
 * functions are declared here
 */
void knnAlgorithm(RunType runType, Dataset *dataset, int numberNeighbors, int numberOfThreads);

void plainKnn      (Dataset *dataset, int numberNeighbors, int firstSample, int lastSample);
void multithreadKnn(Dataset *dataset, int numberNeighbors, int numberOfThreads            );

float euclideanDistance(float *sample1, float *sample2, int numberFeatures);
void doubleSort(float *distances, int *indexes, int numberControl, int numberNeighbors);
int findClass(Dataset *dataset, int *indexes, int numberNeighbors, int *classes);

void *partialKnn(void *args);

#endif
