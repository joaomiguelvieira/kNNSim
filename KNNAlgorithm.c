#include "KNNAlgorithm.h"

void knnAlgorithm(RunType runType, Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int numberOfThreads) {
  switch(runType) {
    case plain:       plainKnn(      dataset, numberNeighbors, distanceMetric, p, 0, dataset->numberClassify); break;
    case multithread: multithreadKnn(dataset, numberNeighbors, distanceMetric, p, numberOfThreads           ); break;
    default:                                                                                                        ;
  }
}

void plainKnn(Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int firstSample, int lastSample) {
  // nothing to do
  if (firstSample == lastSample)
    return;

  // allocate auxiliar vectors just once
  float *distances = (float *) malloc(dataset->numberControl * sizeof(float)); assert(distances != NULL);
  int   *indexes   = (int *)   malloc(dataset->numberControl * sizeof(int)  ); assert(indexes   != NULL);
  int   *classes   = (int *)   malloc(dataset->numberClasses * sizeof(int)  ); assert(classes   != NULL);

  // calculate distances from all classify samples to all control samples
  for (int i = firstSample; i < lastSample; i++) {
    // initialize indexes array and calculate distance from one classify samples to all control samples
    for (int j = 0; j < dataset->numberControl; j++) {
      indexes[j]   = j;
      distances[j] = (distanceMetric == ssd)       ? sumOfSquareDifferences(dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures) :
                     (distanceMetric == euclidean) ? euclideanDistance     (dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures) :
                     (distanceMetric == cosine)    ? cosineDistance        (dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures) :
                     (distanceMetric == chiSquare) ? chiSquareDistance     (dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures) :
                     (distanceMetric == minkowsky) ? minkowskyDistance     (dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures, p) :
                                                     manhattanDistance     (dataset->controlSamples[j], dataset->classifySamples[i], dataset->numberFeatures);
    }

    // find the k closest control samples
    doubleSort(distances, indexes, dataset->numberControl, numberNeighbors);

    // find the class
    dataset->classifyClasses[i] = findClass(dataset, indexes, numberNeighbors, classes);
  }

  // free auxiliar vectors in the end
  free(classes);
  free(indexes);
  free(distances);
}

void *partialKnn(void *args) {
  Dataset *dataset              = (Dataset *)       ((Arguments *) args)->argv[0];
  int numberNeighbors           = (int)            *((Arguments *) args)->argv[1];
  DistanceMetric distanceMetric = (DistanceMetric) *((Arguments *) args)->argv[2];
  int p                         = (int)            *((Arguments *) args)->argv[3];
  int numberOfThreads           = (int)            *((Arguments *) args)->argv[4];
  int threadId                  = (int)            *((Arguments *) args)->argv[5];

  int firstSample = (dataset->numberClassify / numberOfThreads) * threadId;
  int lastSample  = (threadId == numberOfThreads - 1) ? dataset->numberClassify : firstSample + (dataset->numberClassify / numberOfThreads);

  plainKnn(dataset, numberNeighbors, distanceMetric, p, firstSample, lastSample);

  return NULL;
}

void multithreadKnn(Dataset *dataset, int numberNeighbors, DistanceMetric distanceMetric, int p, int numberOfThreads) {
  pthread_t *threads;
  Arguments *args;

  // allocate thread handlers and arguments
  threads = (pthread_t *) malloc(numberOfThreads * sizeof(pthread_t)); assert(threads != NULL);
  args    = (Arguments *) malloc(numberOfThreads * sizeof(Arguments)); assert(args    != NULL);

  // assign arguments and launch all threads
  for (int i = 0; i < numberOfThreads; i++) {
    // total number of arguments
    args[i].argc = 6;

    // allocate space for arguments
    args[i].argv    = (char **) malloc(args[i].argc * sizeof(char *)        ); assert(args[i].argv    != NULL); // list of arguments
    args[i].argv[1] = (char *)  malloc(               sizeof(int)           ); assert(args[i].argv[1] != NULL); // k nearest neighbors
    args[i].argv[2] = (char *)  malloc(               sizeof(DistanceMetric)); assert(args[i].argv[2] != NULL); // distance metric
    args[i].argv[3] = (char *)  malloc(               sizeof(int)           ); assert(args[i].argv[3] != NULL); // p in Minkowsky distance
    args[i].argv[4] = (char *)  malloc(               sizeof(int)           ); assert(args[i].argv[4] != NULL); // number of threads
    args[i].argv[5] = (char *)  malloc(               sizeof(int)           ); assert(args[i].argv[5] != NULL); // thread identifier

    // set thread arguments
    args[i].argv[0]    = (char *) dataset; // dataset
    *(args[i].argv[1]) = numberNeighbors;  // k nearest neighbors
    *(args[i].argv[2]) = distanceMetric;   // distance metric
    *(args[i].argv[3]) = p;                // p in Minkowsky distance
    *(args[i].argv[4]) = numberOfThreads;  // number of threads
    *(args[i].argv[5]) = i;                // thread identifier

    assert(pthread_create(&threads[i], NULL, partialKnn, (void *) &args[i]) == 0);
  }

  // wait for all threads to finish and release allocated resources
  for (int i = 0; i < numberOfThreads; i++) {
    assert(pthread_join(threads[i], NULL) == 0);

    for (int j = 1; j < args[i].argc; j++)
      free(args[i].argv[j]);

    free(args[i].argv);
  }

  free(args);
  free(threads);
}

float sumOfSquareDifferences(float *sample1, float *sample2, int numberFeatures) {
  float difference, distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    difference = sample1[i] - sample2[i];
    distance += difference * difference;
  }

  return distance;
}

float euclideanDistance(float *sample1, float *sample2, int numberFeatures) {
  return sqrt(sumOfSquareDifferences(sample1, sample2, numberFeatures));
}

float cosineDistance(float *sample1, float *sample2, int numberFeatures) {
  float dot_product = 0, magnitude1 = 0, magnitude2 = 0;

  for (int i = 0; i < numberFeatures; i++) {
    dot_product += sample1[i] * sample2[i];
    magnitude1 += sample1[i] * sample1[i];
    magnitude2 += sample2[i] * sample2[i];
  }

  return dot_product / (sqrt(magnitude1) * sqrt(magnitude2));
}

float chiSquareDistance(float *sample1, float *sample2, int numberFeatures) {
  float difference, distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    difference = sample1[i] - sample2[i];
    distance += difference * difference / (sample1[i] + sample2[i]);
  }

  return distance;
}

float minkowskyDistance(float *sample1, float *sample2, int numberFeatures, int p) {
  float distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    distance += powf(fabsf(sample1[i] - sample2[i]), p);
  }

  return powf(distance, 1 / p);
}

float manhattanDistance(float *sample1, float *sample2, int numberFeatures) {
  float distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    distance += fabsf(sample1[i] - sample2[i]);
  }

  return distance;
}

void doubleSort(float *distances, int *indexes, int numberControl, int numberNeighbors) {
  int minimum, aux;

  // sort the minimum k elements
  for (int i = 0; i < numberNeighbors; i++) {
    minimum = i;

    // find the next minimum value
    for (int j = i + 1; j < numberControl; j++)
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

int findClass(Dataset *dataset, int *indexes, int numberNeighbors, int *classes) {
  int maximum = 0;

  // initialize vote array
  for (int i = 0; i < dataset->numberClasses; i++)
    classes[i] = 0;

  // voting process
  for (int i = 0; i < numberNeighbors; i++)
    classes[dataset->controlClasses[indexes[i]]]++;

  // find the most voted class
  for (int i = 1; i < dataset->numberClasses; i++)
    if (classes[i] > classes[maximum])
      maximum = i;

  return maximum;
}
