#include "KNNSim.h"

int main(int argc, char const *argv[]) {
  int numberTraining, numberTesting, numberFeatures, numberClasses, k;
  char runType[BUFLEN];
  int numberOfThreads;
  char inputFilename[BUFLEN], solutionFilename[BUFLEN], saveDataset[BUFLEN], saveSolution[BUFLEN], distanceMetric[BUFLEN];
  int minkowskyP;

#ifndef MACOS
  numberOfThreads = get_nprocs();
#else
  size_t sizeOfInt = sizeof(int);
  sysctlbyname("hw.logicalcpu", &numberOfThreads, &sizeOfInt, NULL, 0);
#endif

  Parser *parser = newParser(argc, (char **) argv);

  // add arguments to the parser
  addArgumentToParser(parser, 1,  "#training",           "",   "integer", "size of the training subset",                                                                 NULL,    &numberTraining,  1);
  addArgumentToParser(parser, 2,  "#testing",            "",   "integer", "size of the testing subset",                                                                  NULL,    &numberTesting,   1);
  addArgumentToParser(parser, 3,  "#features",           "",   "integer", "number of features per each sample",                                                          NULL,    &numberFeatures,  1);
  addArgumentToParser(parser, 4,  "#classes",            "",   "integer", "number of different classes in the training subset (smaller than #training)",                 NULL,    &numberClasses,   1);
  addArgumentToParser(parser, 5,  "#neighbors",          "",   "integer", "(k) number of closest neighbors needed to testing a sample",                                  NULL,    &k,               1);
  addArgumentToParser(parser, -1, "--run-type",          "-r", "string",  "run-type plain, multithread or cuda",                                                         "plain", runType,          0);
  addArgumentToParser(parser, -1, "--number-of-threads", "-t", "integer", "number of threads",                                                                           NULL,    &numberOfThreads, 0);
  addArgumentToParser(parser, -1, "--input-file",        "-f", "string",  "binary file that includes training samples, testing samples and classes",                     "\0",    inputFilename,    0);
  addArgumentToParser(parser, -1, "--solution-file",     "-s", "string",  "file with the actual classes of the classified samples that allows calculating kNN accuracy", "\0",    solutionFilename, 0);
  addArgumentToParser(parser, -1, "--save-dataset",      "-D", "string",  "save the operated dataset to a file under this designation",                                  "\0",    saveDataset,      0);
  addArgumentToParser(parser, -1, "--save-solution",     "-S", "string",  "save the calculated solution to a file under this designation",                               "\0",    saveSolution,     0);
  addArgumentToParser(parser, -1, "--distance-metric",   "-d", "string",  "distance metric ssd, euclidean, cosine, chi-square, minkowsky or manhattan",                  "ssd",   distanceMetric,   0);
  addArgumentToParser(parser, -1, "--minkowsky-p",       "-p", "integer", "parameter p of minkowsky distance",                                                           "2",     &minkowskyP,      0);

  // parse all arguments
  if (parseAllArguments(parser)) {
    usage(parser);
    exit(0);
  }

  // create knnDataset and knnClassifier structures
  KNNDataset *knnDataset = newKNNDataset(numberTraining, numberTesting, numberFeatures, numberClasses);
  KNNClassifier *knnClassifier = newKNNClassifier(k);

  strcpy(knnClassifier->runType, runType);
  knnClassifier->numberOfThreads = numberOfThreads;
  strcpy(knnDataset->inputFilename, inputFilename);
  strcpy(knnDataset->solutionFilename, solutionFilename);
  strcpy(knnDataset->saveDataset, saveDataset);
  strcpy(knnDataset->saveSolution, saveSolution);
  strcpy(knnClassifier->distanceMetric->metricName, distanceMetric);
  knnClassifier->distanceMetric->metric = getPointerToMetric(distanceMetric);
  knnClassifier->distanceMetric->p = minkowskyP;

  // create knnDataset
  if (!strcmp(knnDataset->inputFilename, "\0"))
    randKNNDataset(knnDataset);
  else
    loadKNNDataset(knnDataset);

  struct timeval startKNNAlgorithm, endKNNAlgorithm;

#ifdef CUDA
  // initialize cuda device before using it
  assert(cudaFree(0) == cudaSuccess);
#endif

  gettimeofday(&startKNNAlgorithm, NULL);
  knnAlgorithm(knnDataset, knnClassifier);
  gettimeofday(&endKNNAlgorithm, NULL);

  if (!strcmp(knnClassifier->runType, "plain"))
    knnClassifier->totalExecutionTime = knnClassifier->distanceComputation + knnClassifier->knnFinder + knnClassifier->queryLabelFinder;
  else
    knnClassifier->totalExecutionTime = getElapsedTime(startKNNAlgorithm, endKNNAlgorithm);

  if(!strcmp(knnClassifier->runType, "cuda"))
    knnClassifier->cudaTransferTime = knnClassifier->totalExecutionTime - knnClassifier->cudaKernelTime;

  if (strcmp(knnDataset->saveDataset, "\0"))
    saveDatasetToFile(knnDataset);
  if (strcmp(knnDataset->saveSolution, "\0"))
    saveSolutionToFile(knnDataset);

  printKNNClassifierConfiguration(knnClassifier);
  printKNNDatasetConfiguration(knnDataset);
  printPerformanceResults(knnClassifier);
  if (strcmp(knnDataset->inputFilename, "\0") && strcmp(knnDataset->solutionFilename, "\0"))
    printKNNAccuracyResults(knnDataset);

  deleteKNNClassifier(knnClassifier);
  deleteKNNDataset(knnDataset);
  deleteParser(parser);

  return 0;
}
