#ifndef __DISTANCE_METRICS_HEADER__
#define __DISTANCE_METRICS_HEADER__

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Common.h"

typedef struct DistanceMetric_ {
  float (*metric)(float *, float *, int, struct DistanceMetric_ *);
  char metricName[BUFLEN];
  int p;
} DistanceMetric;

DistanceMetric *newDistanceMetric();
void deleteDistanceMetric(DistanceMetric *distanceMetric);
void *getPointerToMetric(char *metricName);

// distance metrics
float sumOfSquaredDifferences (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);
float euclideanDistance       (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);
float cosineDistance          (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);
float chiSquareDistance       (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);
float minkowskyDistance       (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);
float manhattanDistance       (float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric);

#endif
