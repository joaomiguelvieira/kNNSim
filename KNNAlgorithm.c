#include "KNNAlgorithm.h"

void knnAlgorithm(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
       if (!strcmp(knnClassifier->runType, "plain"))       plainKnn(      knnDataset, knnClassifier, 0, knnDataset->numberTesting);
  else if (!strcmp(knnClassifier->runType, "multithread")) multithreadKnn(knnDataset, knnClassifier);
}

void plainKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier, int firstSample, int lastSample) {
  // nothing to do
  if (firstSample == lastSample)
    return;

  // allocate auxiliar vectors
  float *distances = (float *) malloc(knnDataset->numberTraining * sizeof(float)); assert(distances != NULL);
  int   *indexes   = (int *)   malloc(knnDataset->numberTraining * sizeof(int)  ); assert(indexes   != NULL);
  int   *classes   = (int *)   malloc(knnDataset->numberClasses  * sizeof(int)  ); assert(classes   != NULL);

  struct timeval startDistanceComputation, endDistanceComputation;
  struct timeval startKNNFinder, endKNNFinder;
  struct timeval startQueryLabelFinder, endQueryLabelFinder;

  // calculate distances from all testing samples to all training samples
  for (int i = firstSample; i < lastSample; i++) {
    // initialize indexes array and calculate distance from one testing samples to all training samples
    gettimeofday(&startDistanceComputation, NULL);
    for (int j = 0; j < knnDataset->numberTraining; j++) {
      indexes[j]   = j;
      distances[j] = knnClassifier->distanceMetric->metric(knnDataset->trainingSamples[j], knnDataset->testingSamples[i], knnDataset->numberFeatures, knnClassifier->distanceMetric);
    }
    gettimeofday(&endDistanceComputation, NULL);

    // find the k closest training samples
    gettimeofday(&startKNNFinder, NULL);
    doubleSort(knnDataset, knnClassifier, distances, indexes);
    gettimeofday(&endKNNFinder, NULL);

    // find the class
    gettimeofday(&startQueryLabelFinder, NULL);
    knnDataset->testingClasses[i] = findClass(knnDataset, knnClassifier, indexes, classes);
    gettimeofday(&endQueryLabelFinder, NULL);

    if (!strcmp(knnClassifier->runType, "plain")) {
      knnClassifier->distanceComputation += getElapsedTime(startDistanceComputation, endDistanceComputation);
      knnClassifier->knnFinder += getElapsedTime(startKNNFinder, endKNNFinder);
      knnClassifier->queryLabelFinder += getElapsedTime(startQueryLabelFinder, endQueryLabelFinder);
    }
  }

  // free auxiliar vectors in the end
  free(classes);
  free(indexes);
  free(distances);
}

void *partialKnn(void *args) {
  KNNDataset *knnDataset        = (KNNDataset *)    ((Arguments *) args)->argv[0];
  KNNClassifier *knnClassifier  = (KNNClassifier *) ((Arguments *) args)->argv[1];
  int threadId                  = (int)             *((Arguments *) args)->argv[2];

  int firstSample = (knnDataset->numberTesting / knnClassifier->numberOfThreads) * threadId;
  int lastSample  = (threadId == knnClassifier->numberOfThreads - 1) ? knnDataset->numberTesting : firstSample + (knnDataset->numberTesting / knnClassifier->numberOfThreads);

  plainKnn(knnDataset, knnClassifier, firstSample, lastSample);

  return NULL;
}

void multithreadKnn(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
  pthread_t *threads;
  Arguments *args;

  // allocate thread handlers and arguments
  threads = (pthread_t *) malloc(knnClassifier->numberOfThreads * sizeof(pthread_t)); assert(threads != NULL);
  args    = (Arguments *) malloc(knnClassifier->numberOfThreads * sizeof(Arguments)); assert(args    != NULL);

  // assign arguments and launch all threads
  for (int i = 0; i < knnClassifier->numberOfThreads; i++) {
    // total number of arguments
    args[i].argc = 3;

    // allocate space for arguments
    args[i].argv    = (char **) malloc(args[i].argc * sizeof(char *)); assert(args[i].argv    != NULL); // list of arguments
    args[i].argv[2] = (char *)  malloc(               sizeof(int)   ); assert(args[i].argv[2] != NULL); // thread identifier

    // set thread arguments
    args[i].argv[0] = (char *) knnDataset;    // knnDataset
    args[i].argv[1] = (char *) knnClassifier; // knnClassifier
    *(args[i].argv[2]) = i;                   // thread identifier

    assert(!pthread_create(&threads[i], NULL, partialKnn, (void *) &args[i]));
  }

  // wait for all threads to finish and release allocated resources
  for (int i = 0; i < knnClassifier->numberOfThreads; i++) {
    assert(!pthread_join(threads[i], NULL));

    for (int j = 2; j < args[i].argc; j++)
      free(args[i].argv[j]);

    free(args[i].argv);
  }

  free(args);
  free(threads);
}

void doubleSort(KNNDataset *knnDataset, KNNClassifier *knnClassifier, float *distances, int *indexes) {
  int minimum, aux;

  // sort the minimum k elements
  for (int i = 0; i < knnClassifier->k; i++) {
    minimum = i;

    // find the next minimum value
    for (int j = i + 1; j < knnDataset->numberTraining; j++)
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

int findClass(KNNDataset *knnDataset, KNNClassifier *knnClassifier, int *indexes, int *classes) {
  int maximum = 0;

  // initialize vote array
  for (int i = 0; i < knnDataset->numberClasses; i++)
    classes[i] = 0;

  // voting process
  for (int i = 0; i < knnClassifier->k; i++)
    classes[knnDataset->trainingClasses[indexes[i]]]++;

  // find the most voted class
  for (int i = 1; i < knnDataset->numberClasses; i++)
    if (classes[i] > classes[maximum])
      maximum = i;

  return maximum;
}

float calculateAccuracy(KNNDataset *knnDataset, KNNClassifier *knnClassifier) {
  float right = 0, wrong = 0;
  int class;
  char line[BUFLEN];

  FILE *solutionFile = fopen(knnDataset->solutionFilename, "r");
  assert(solutionFile != NULL);

  for (int i = 0; i < knnDataset->numberTesting; i++) {
    assert(fgets(line, BUFLEN, solutionFile) != NULL);
    assert(sscanf(line, "%d", &class) == 1);

    if (class == knnDataset->testingClasses[i])
      right++;
    else
      wrong++;
  }

  return right * 100 / (right + wrong);
}
