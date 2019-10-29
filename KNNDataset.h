#ifndef __DATASET_HEADER__
#define __DATASET_HEADER__

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Common.h"

#define MAX_NUMBER  10
#define FIXED_POINT 100

typedef struct KNNDataset_ {
  int numberTraining, numberTesting, numberFeatures, numberClasses;

  // training subset
  float **trainingSamples;
  int    *trainingClasses;

  // testing subset
  float **testingSamples;
  int    *testingClasses;

  // file names
  char inputFilename[BUFLEN], solutionFilename[BUFLEN];
} KNNDataset;

KNNDataset *newKNNDataset(int numberTraining, int numberTesting, int numberFeatures, int numberClasses);
void loadKNNDataset(KNNDataset *knnDataset);
int checkKNNDatasetIntegrity(KNNDataset *kNNDataset);
void randKNNDataset(KNNDataset *knnDataset);
void deleteKNNDataset(KNNDataset *kNNDataset);
void printKNNDatasetConfiguration(KNNDataset *knnDataset);

#endif
