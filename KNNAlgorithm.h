#ifndef __KNNALGORITHM_HEADER__
#define __KNNALGORITHM_HEADER__

/*
 * libraries includes go here
 */
#include <math.h>
#include <pthread.h>
#include "Dataset.h"
#include "Common.h"

 /*
  * structures are declared here
  */
typedef enum RunType_ {plain, multithread} RunType;
typedef enum DistanceMetric_ {ssd, euclidean, cosine, chiSquare, minkowsky, manhattan} DistanceMetric;
typedef struct Arguments_ {
  int argc;
  char **argv;
} Arguments;

/*
 * functions are declared here
 */
void knnAlgorithm(RunType runType, Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int numberOfThreads, int profile);

void plainKnn      (Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int firstSample, int lastSample, int profile);
void multithreadKnn(Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int numberOfThreads            );

float sumOfSquareDifferences(float *sample1, float *sample2, int numberFeatures);
float euclideanDistance     (float *sample1, float *sample2, int numberFeatures);
float cosineDistance        (float *sample1, float *sample2, int numberFeatures);
float chiSquareDistance     (float *sample1, float *sample2, int numberFeatures);
float minkowskyDistance     (float *sample1, float *sample2, int numberFeatures, int p);
float manhattanDistance     (float *sample1, float *sample2, int numberFeatures);

void doubleSort(float *distances, int *indexes, int numberControl, int numberNeighbors);
int findClass(Dataset *dataset, int *indexes, int numberNeighbors, int *classes);

float calculateAccuracy(Dataset *dataset, char *solutionFilename);

void *partialKnn(void *args);

#endif
