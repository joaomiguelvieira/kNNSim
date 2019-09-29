#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__

#include <sys/time.h>
#include <unistd.h>

/*
 * functions are declared here
 */
double getElapsedTime(struct timeval startTime, struct timeval endTime);

#endif
