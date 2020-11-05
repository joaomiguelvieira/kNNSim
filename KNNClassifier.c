#include "KNNClassifier.h"

KNNClassifier *newKNNClassifier(int k) {
  // allocate knnClassifier
  KNNClassifier *knnClassifier = (KNNClassifier *) malloc(sizeof(KNNClassifier));
  assert(knnClassifier != NULL);

  // allocate distance metric parameters
  knnClassifier->distanceMetric = newDistanceMetric();

  // default k
  knnClassifier->k = k;

  // initialize times
  knnClassifier->distanceComputation = 0;
  knnClassifier->knnFinder           = 0;
  knnClassifier->queryLabelFinder    = 0;
  knnClassifier->totalExecutionTime  = 0;

  // initilize cuda parameters
  knnClassifier->cudaKernelTime = 0;
  knnClassifier->cudaNumberOfBlocks = 0;
  knnClassifier->cudaThreadsPerBlock = 0;

  // hostname
  gethostname(knnClassifier->hostname, (size_t) BUFLEN);

  return knnClassifier;
}

void deleteKNNClassifier(KNNClassifier *knnClassifier) {
  deleteDistanceMetric(knnClassifier->distanceMetric);
  free(knnClassifier);
}

void printKNNClassifierConfiguration(KNNClassifier *knnClassifier) {
  printf("\033[1m[CLASSIFIER SUMMARY]:\033[0m\n");
  printf("|_ \033[1mhostname:\033[0m %s\n", knnClassifier->hostname);
  printf("|_ \033[1mrun-type:\033[0m %s\n", knnClassifier->runType);
  if (!strcmp(knnClassifier->runType, "multithread"))
    printf("   |_ \033[1m#threads:\033[0m %d\n", knnClassifier->numberOfThreads);
  printf("|_ \033[1mmetric:\033[0m %s\n", knnClassifier->distanceMetric->metricName);
  if (!strcmp(knnClassifier->distanceMetric->metricName, "minkowsky"))
    printf("   |_ \033[1mp:\033[0m %d\n", knnClassifier->distanceMetric->p);
  printf("|_ \033[1mk:\033[0m %d\n", knnClassifier->k);
}

void printPerformanceResults(KNNClassifier *knnClassifier) {
  printf("\033[1m[PERFORMANCE RESULTS]:\033[0m\n");
  printf("|_ \033[1mexecution time:\033[0m\n");
  if (!strcmp(knnClassifier->runType, "plain")) {
    printf("   |_ \033[1mdistance computation [s]:\033[0m %f (%.2f%%)\n", knnClassifier->distanceComputation, knnClassifier->distanceComputation * 100 / knnClassifier->totalExecutionTime);
    printf("   |_ \033[1mKNN finder [s]:\033[0m %f (%.2f%%)\n", knnClassifier->knnFinder, knnClassifier->knnFinder * 100 / knnClassifier->totalExecutionTime);
    printf("   |_ \033[1mquery label finder [s]:\033[0m %f (%.2f%%)\n", knnClassifier->queryLabelFinder, knnClassifier->queryLabelFinder * 100 / knnClassifier->totalExecutionTime);
  }
  printf("   |_ \033[1mtotal [s]:\033[0m %f\n", knnClassifier->totalExecutionTime);
}
