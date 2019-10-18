#include "Dataset.h"

Dataset *initDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses) {
  Dataset *dataset;

  // allocate main structure
  dataset = (Dataset *) malloc(sizeof(Dataset));
  assert(dataset != NULL);

  // allocate training subset
  dataset->trainingSamples    = (float **) malloc(numberTraining                  * sizeof(float *)); assert(dataset->trainingSamples    != NULL);
  dataset->trainingSamples[0] = (float *)  malloc(numberTraining * numberFeatures * sizeof(float)  ); assert(dataset->trainingSamples[0] != NULL);
  for (int i = 1; i < numberTraining; i++)
    dataset->trainingSamples[i] = dataset->trainingSamples[i - 1] + numberFeatures;

  // allocate testing subset
  dataset->testingSamples    = (float **) malloc(numberTesting                  * sizeof(float *)); assert(dataset->testingSamples    != NULL);
  dataset->testingSamples[0] = (float *)  malloc(numberTesting * numberFeatures * sizeof(float)  ); assert(dataset->testingSamples[0] != NULL);
  for (int i = 1; i < numberTesting; i++)
    dataset->testingSamples[i] = dataset->testingSamples[i - 1] + numberFeatures;

  // allocate classes vectors
  dataset->trainingClasses = (int *) malloc(numberTraining * sizeof(int)); assert(dataset->trainingClasses != NULL);
  dataset->testingClasses  = (int *) malloc(numberTesting  * sizeof(int)); assert(dataset->testingClasses  != NULL);

  // initialize testing classes
  for (int i = 0; i < numberTesting; i++)
    dataset->testingClasses[i] = -1;

  // dataset parameters
  dataset->numberTraining = numberTraining;
  dataset->numberTesting  = numberTesting;
  dataset->numberFeatures = numberFeatures;
  dataset->numberClasses  = numberClasses;

  // return the allocated dataset
  return dataset;
}

Dataset *loadDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses, char *inputFilename) {
  // open input file
  FILE *inputFile = fopen(inputFilename, "rb");
  assert(inputFile != NULL);

  // allocate dataset
  Dataset *dataset = initDataset(numberTraining, numberTesting, numberFeatures, numberClasses);

  // read dataset from file
  assert(fread(dataset->trainingSamples[0], sizeof(float), numberTraining * numberFeatures, inputFile));
  assert(fread(dataset->testingSamples[0],  sizeof(float), numberTesting  * numberFeatures, inputFile));
  assert(fread(dataset->trainingClasses,    sizeof(int),   numberTraining,                  inputFile));

  // close input file
  fclose(inputFile);

  return dataset;
}

int checkDatasetIntegrity(Dataset *dataset) {
  for (int i = 0; i < dataset->numberTraining; i++)
    if (dataset->trainingClasses[i] < 0 || dataset->trainingClasses[i] >= dataset->numberClasses) {
      fprintf(stderr, "Dataset is corrupt! Classes have to be in the range [0, %d[. Specified class is %d.\n", dataset->numberClasses, dataset->trainingClasses[i]);
      return 1;
    }

    return 0;
}

Dataset *randDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses) {
  // allocate dataset
  Dataset *dataset = initDataset(numberTraining, numberTesting, numberFeatures, numberClasses);

  // seed the random number generator
  srand(time(NULL));

  // generate random data for training subset
  for (int i = 0; i < numberTraining; i++) {
    // generate random samples
    for (int j = 0; j < numberFeatures; j++) {
      dataset->trainingSamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      dataset->trainingSamples[i][j] /= FIXED_POINT;
    }

    // assign a class randomly
    dataset->trainingClasses[i] = rand() % numberClasses;
  }

  // generate random samples for the testing subset
  for (int i = 0; i < numberTesting; i++) {
    for (int j = 0; j < numberFeatures; j++) {
      dataset->testingSamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      dataset->testingSamples[i][j] /= FIXED_POINT;
    }
  }

  return dataset;
}

void destroyDataset(Dataset *dataset) {
  free(dataset->testingClasses);
  free(dataset->trainingClasses);
  free(dataset->testingSamples[0]);
  free(dataset->testingSamples);
  free(dataset->trainingSamples[0]);
  free(dataset->trainingSamples);
  free(dataset);
}
