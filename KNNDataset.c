#include "KNNDataset.h"

KNNDataset *newKNNDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses) {
  KNNDataset *knnDataset;

  // allocate main structure
  knnDataset = (KNNDataset *) malloc(sizeof(KNNDataset));
  assert(knnDataset != NULL);

  // allocate training subset
  knnDataset->trainingSamples    = (float **) malloc(numberTraining                  * sizeof(float *)); assert(knnDataset->trainingSamples    != NULL);
  knnDataset->trainingSamples[0] = (float *)  malloc(numberTraining * numberFeatures * sizeof(float)  ); assert(knnDataset->trainingSamples[0] != NULL);
  for (int i = 1; i < numberTraining; i++)
    knnDataset->trainingSamples[i] = knnDataset->trainingSamples[i - 1] + numberFeatures;

  // allocate testing subset
  knnDataset->testingSamples    = (float **) malloc(numberTesting                  * sizeof(float *)); assert(knnDataset->testingSamples    != NULL);
  knnDataset->testingSamples[0] = (float *)  malloc(numberTesting * numberFeatures * sizeof(float)  ); assert(knnDataset->testingSamples[0] != NULL);
  for (int i = 1; i < numberTesting; i++)
    knnDataset->testingSamples[i] = knnDataset->testingSamples[i - 1] + numberFeatures;

  // allocate classes vectors
  knnDataset->trainingClasses = (int *) malloc(numberTraining * sizeof(int)); assert(knnDataset->trainingClasses != NULL);
  knnDataset->testingClasses  = (int *) malloc(numberTesting  * sizeof(int)); assert(knnDataset->testingClasses  != NULL);

  // initialize testing classes
  for (int i = 0; i < numberTesting; i++)
    knnDataset->testingClasses[i] = -1;

  // knnDataset parameters
  knnDataset->numberTraining = numberTraining;
  knnDataset->numberTesting  = numberTesting;
  knnDataset->numberFeatures = numberFeatures;
  knnDataset->numberClasses  = numberClasses;

  // return the allocated knnDataset
  return knnDataset;
}

void loadKNNDataset(KNNDataset *knnDataset) {
  // open input file
  FILE *inputFile = fopen(knnDataset->inputFilename, "rb");
  assert(inputFile != NULL);

  // read knnDataset from file
  assert(fread(knnDataset->trainingSamples[0], sizeof(float), knnDataset->numberTraining * knnDataset->numberFeatures, inputFile));
  assert(fread(knnDataset->testingSamples[0],  sizeof(float), knnDataset->numberTesting  * knnDataset->numberFeatures, inputFile));
  assert(fread(knnDataset->trainingClasses,    sizeof(int),   knnDataset->numberTraining,                              inputFile));

  // close input file
  fclose(inputFile);
}

int checkKNNDatasetIntegrity(KNNDataset *knnDataset) {
  for (int i = 0; i < knnDataset->numberTraining; i++)
    if (knnDataset->trainingClasses[i] < 0 || knnDataset->trainingClasses[i] >= knnDataset->numberClasses) {
      fprintf(stderr, "KNNDataset is corrupt! Classes have to be in the range [0, %d[. Specified class is %d.\n", knnDataset->numberClasses, knnDataset->trainingClasses[i]);
      return 1;
    }

    return 0;
}

void randKNNDataset(KNNDataset * knnDataset) {
  // seed the random number generator
  srand(time(NULL));

  // generate random data for training subset
  for (int i = 0; i < knnDataset->numberTraining; i++) {
    // generate random samples
    for (int j = 0; j < knnDataset->numberFeatures; j++) {
      knnDataset->trainingSamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      knnDataset->trainingSamples[i][j] /= FIXED_POINT;
    }

    // assign a class randomly
    knnDataset->trainingClasses[i] = rand() % knnDataset->numberClasses;
  }

  // generate random samples for the testing subset
  for (int i = 0; i < knnDataset->numberTesting; i++) {
    for (int j = 0; j < knnDataset->numberFeatures; j++) {
      knnDataset->testingSamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      knnDataset->testingSamples[i][j] /= FIXED_POINT;
    }
  }
}

void deleteKNNDataset(KNNDataset *knnDataset) {
  free(knnDataset->testingClasses);
  free(knnDataset->trainingClasses);
  free(knnDataset->testingSamples[0]);
  free(knnDataset->testingSamples);
  free(knnDataset->trainingSamples[0]);
  free(knnDataset->trainingSamples);
  free(knnDataset);
}

void printKNNDatasetConfiguration(KNNDataset *knnDataset) {
  printf("\033[1m[DATASET SUMMARY]:\033[0m\n");
  printf("|_ \033[1mtraining:\033[0m %d\n", knnDataset->numberTraining);
  printf("|_ \033[1mtesting:\033[0m %d\n", knnDataset->numberTesting);
  printf("|_ \033[1mfeatures:\033[0m %d\n", knnDataset->numberFeatures);
  printf("|_ \033[1mclasses:\033[0m %d\n", knnDataset->numberClasses);
  if (strcmp(knnDataset->inputFilename, "\0"))
    printf("|_ \033[1minput file:\033[0m %s\n", knnDataset->inputFilename);
  if (strcmp(knnDataset->solutionFilename, "\0"))
  printf("|_ \033[1msolution file:\033[0m %s\n", knnDataset->solutionFilename);
}