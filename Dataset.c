#include "Dataset.h"

Dataset *initDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses) {
  Dataset *dataset;

  // allocate main structure
  dataset = (Dataset *) malloc(sizeof(Dataset));
  assert(dataset != NULL);

  // allocate control subset
  dataset->controlSamples    = (float **) malloc(numberControl                  * sizeof(float *)); assert(dataset->controlSamples    != NULL);
  dataset->controlSamples[0] = (float *)  malloc(numberControl * numberFeatures * sizeof(float)  ); assert(dataset->controlSamples[0] != NULL);
  for (int i = 1; i < numberControl; i++)
    dataset->controlSamples[i] = dataset->controlSamples[i - 1] + numberFeatures;

  // allocate classify subset
  dataset->classifySamples    = (float **) malloc(numberClassify                  * sizeof(float *)); assert(dataset->classifySamples    != NULL);
  dataset->classifySamples[0] = (float *)  malloc(numberClassify * numberFeatures * sizeof(float)  ); assert(dataset->classifySamples[0] != NULL);
  for (int i = 1; i < numberClassify; i++)
    dataset->classifySamples[i] = dataset->classifySamples[i - 1] + numberFeatures;

  // allocate classes vectors
  dataset->controlClasses  = (int *) malloc(numberControl  * sizeof(int)); assert(dataset->controlClasses  != NULL);
  dataset->classifyClasses = (int *) malloc(numberClassify * sizeof(int)); assert(dataset->classifyClasses != NULL);

  // initialize classify classes
  for (int i = 0; i < numberClassify; i++)
    dataset->classifyClasses[i] = -1;

  // dataset parameters
  dataset->numberControl  = numberControl;
  dataset->numberClassify = numberClassify;
  dataset->numberFeatures = numberFeatures;
  dataset->numberClasses  = numberClasses;

  // return the allocated dataset
  return dataset;
}

Dataset *loadDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses, char *inputFilename) {
  // open input file
  FILE *inputFile = fopen(inputFilename, "rb");
  assert(inputFile != NULL);

  // allocate dataset
  Dataset *dataset = initDataset(numberControl, numberClassify, numberFeatures, numberClasses);

  // read dataset from file
  fread(dataset->controlSamples[0],  sizeof(float), numberControl  * numberFeatures, inputFile);
  fread(dataset->classifySamples[0], sizeof(float), numberClassify * numberFeatures, inputFile);
  fread(dataset->controlClasses,     sizeof(int),   numberControl,                   inputFile);

  // close input file
  fclose(inputFile);

  return dataset;
}

Dataset *randDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses) {
  // allocate dataset
  Dataset *dataset = initDataset(numberControl, numberClassify, numberFeatures, numberClasses);

  // seed the random number generator
  srand(time(NULL));

  // generate random data for control subset
  for (int i = 0; i < numberControl; i++) {
    // generate random samples
    for (int j = 0; j < numberFeatures; j++) {
      dataset->controlSamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      dataset->controlSamples[i][j] /= FIXED_POINT;
    }

    // assign a class randomly
    dataset->controlClasses[i] = rand() % numberClasses;
  }

  // generate random samples for the classify subset
  for (int i = 0; i < numberClassify; i++) {
    for (int j = 0; j < numberFeatures; j++) {
      dataset->classifySamples[i][j] = (rand() % (MAX_NUMBER * FIXED_POINT));
      dataset->classifySamples[i][j] /= FIXED_POINT;
    }
  }

  return dataset;
}

void destroyDataset(Dataset *dataset) {
  free(dataset->classifyClasses);
  free(dataset->controlClasses);
  free(dataset->classifySamples[0]);
  free(dataset->classifySamples);
  free(dataset->controlSamples[0]);
  free(dataset->controlSamples);
  free(dataset);
}
