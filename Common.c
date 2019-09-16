#include "Common.h"

double getElapsedTime(struct timeval startTime, struct timeval endTime) {
  return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.00;
}
