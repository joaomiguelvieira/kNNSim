#ifndef __KNNSIM_HEADER__
#define __KNNSIM_HEADER__

/*
 * libraries includes go here
 */
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include <assert.h>
#include "Dataset.h"
#include "KNNAlgorithm.h"
#include "Common.h"

#ifndef MACOS
#include <sys/sysinfo.h>
#else
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#define BUFLEN 100

/*
 * functions are declared here
 */
void usage(char *executable);
float calculateAccuracy(Dataset *dataset, char *solutionFilename);

#endif
