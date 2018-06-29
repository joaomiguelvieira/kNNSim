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
  int numberControl, numberClassify, numberFeatures, numberClasses;

  // control set
  float **controlSamples;
  int    *controlClasses;

  // classify set
  float **classifySamples;
  int    *classifyClasses;
} Dataset;

/*
 * functions are declared here
 */
Dataset *initDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses);
Dataset *loadDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses, char *inputFilename);
Dataset *randDataset(int numberControl, int numberClassify, int numberFeatures, int numberClasses);

void destroyDataset(Dataset *dataset);

#endif
