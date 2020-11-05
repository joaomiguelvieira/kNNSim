#ifndef __KNNSIM_HEADER__
#define __KNNSIM_HEADER__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "KNNDataset.h"
#include "KNNClassifier.h"
#include "KNNAlgorithm.h"
#include "Common.h"
#include "Parser.h"

// required for getting the number of threads
#ifndef MACOS
#include <sys/sysinfo.h>
#else
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#ifndef CUDA
#include <cuda.h>
#include <cuda_runtime.h>
#endif

#endif
