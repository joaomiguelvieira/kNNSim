#ifndef __DATASET_HEADER__
#define __DATASET_HEADER__

/*
 * libraries includes go here
 */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * defines go here
 */
#define MAX_NUMBER  10
#define FIXED_POINT 100

/*
 * structures are declared here
 */
typedef struct Dataset_ {
  int numberTraining, numberTesting, numberFeatures, numberClasses;

  // training subset
  float **trainingSamples;
  int    *trainingClasses;

  // testing subset
  float **testingSamples;
  int    *testingClasses;
} Dataset;

/*
 * functions are declared here
 */
Dataset *initDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses);
Dataset *loadDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses, char *inputFilename);
int      checkDatasetIntegrity(Dataset *dataset);
Dataset *randDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses);
void     destroyDataset(Dataset *dataset);

#endif
