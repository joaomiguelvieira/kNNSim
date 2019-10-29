#include "DistanceMetrics.h"

DistanceMetric *newDistanceMetric() {
  DistanceMetric *distanceMetric = (DistanceMetric *) malloc(sizeof(DistanceMetric));
  assert(distanceMetric != NULL);

  // defaults
  distanceMetric->metric = (void *) &sumOfSquaredDifferences;
  sprintf(distanceMetric->metricName, "ssd");
  distanceMetric->p = 2;

  return distanceMetric;
}

void deleteDistanceMetric(DistanceMetric *distanceMetric) {
  free(distanceMetric);
}

void *getPointerToMetric(char *metricName) {
       if(!strcmp(metricName, "ssd"))        return (void *) &sumOfSquaredDifferences;
  else if(!strcmp(metricName, "euclidean"))  return (void *) &euclideanDistance;
  else if(!strcmp(metricName, "cosine"))     return (void *) &cosineDistance;
  else if(!strcmp(metricName, "chi-square")) return (void *) &chiSquareDistance;
  else if(!strcmp(metricName, "minkowsky"))  return (void *) &minkowskyDistance;
  else if(!strcmp(metricName, "manhattan"))  return (void *) &manhattanDistance;
  else                                       return NULL;
}

float sumOfSquaredDifferences(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  float difference, distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    difference = sample1[i] - sample2[i];
    distance += difference * difference;
  }

  return distance;
}

float euclideanDistance(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  return sqrt(sumOfSquaredDifferences(sample1, sample2, numberFeatures, distanceMetric));
}

float cosineDistance(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  float dot_product = 0, magnitude1 = 0, magnitude2 = 0;

  for (int i = 0; i < numberFeatures; i++) {
    dot_product += sample1[i] * sample2[i];
    magnitude1 += sample1[i] * sample1[i];
    magnitude2 += sample2[i] * sample2[i];
  }

  return dot_product / (sqrt(magnitude1) * sqrt(magnitude2));
}

float chiSquareDistance(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  float difference, distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    difference = sample1[i] - sample2[i];
    distance += difference * difference / (sample1[i] + sample2[i]);
  }

  return distance;
}

float minkowskyDistance(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  float distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    distance += powf(fabsf(sample1[i] - sample2[i]), distanceMetric->p);
  }

  return powf(distance, 1.0 / (float) distanceMetric->p);
}

float manhattanDistance(float *sample1, float *sample2, int numberFeatures, DistanceMetric *distanceMetric) {
  float distance = 0;

  for (int i = 0; i < numberFeatures; i++) {
    distance += fabsf(sample1[i] - sample2[i]);
  }

  return distance;
}
