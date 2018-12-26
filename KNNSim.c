#include "KNNSim.h"

int main(int argc, char const *argv[]) {
  int numberControl, numberClassify, numberFeatures, numberClasses, numberNeighbors;
  Dataset *dataset;
  struct timeval startTime, endTime;

  // optional arguments
  RunType runType         = plain;        // type of run
  int     inputFile       = 0;            // optional binary input file
  char    *inputFilename  = NULL;
  int     numberOfThreads;
  char    hostname[BUFLEN];

  // default number of threads
#ifndef MACOS
  numberOfThreads = get_nprocs();
#else
  size_t sizeOfInt = sizeof(int);
  sysctlbyname("machdep.cpu.core_count", &numberOfThreads, &sizeOfInt, NULL, 0);
#endif

  gethostname(hostname, (size_t) BUFLEN);

  // checking for right number of arguments
  if (argc < 6) {
    usage((char *) argv[0]);

    exit(0);
  }

  // parsing arguments
  numberControl   = atoi(argv[1]);
  numberClassify  = atoi(argv[2]);
  numberFeatures  = atoi(argv[3]);
  numberClasses   = atoi(argv[4]);
  numberNeighbors = atoi(argv[5]);

  for (int i = 6; i < argc - 1; i += 2) {
    // specified type of run
    if (!strcmp(argv[i], "--run-type") || !strcmp(argv[i], "-r")) {
      if     (!strcmp(argv[i + 1], "plain"      )) runType = plain;
      else if(!strcmp(argv[i + 1], "multithread")) runType = multithread;
    }
    // specified input file
    else if (!strcmp(argv[i], "--input-file") || !strcmp(argv[i], "-f")) {
      inputFile     = 1;
      inputFilename = (char *) argv[i + 1];
    }
    // specify the number of threads
    else if (!strcmp(argv[i], "--number-of-threads") || !strcmp(argv[i], "-t")) {
      numberOfThreads = atoi(argv[i + 1]);
    }
    // optional argument not recognized
    else {
      printf("Optional argument %s is invalid and will be ignored\n", argv[i]);
      i--;
    }
  }

  // correct number of threads if run type does not allow several threads
  if (runType != multithread)
    numberOfThreads = 1;

  // if an input file is provided, use it; otherwise generate the dataset
  if (inputFile == 1)
    dataset = loadDataset(numberControl, numberClassify, numberFeatures, numberClasses, inputFilename);
  else
    dataset = randDataset(numberControl, numberClassify, numberFeatures, numberClasses);

  // execute knn algorithm
  gettimeofday(&startTime, NULL);
  knnAlgorithm(runType, dataset, numberNeighbors, numberOfThreads);
  gettimeofday(&endTime, NULL);

  // print summary
  printf("         Hostname: %s\n", hostname);
  printf("         Run type: %d\n", runType);
  printf("Number of threads: %d\n", numberOfThreads);
  printf(" Time elapsed [s]: %lf\n", getElapsedTime(startTime, endTime));

  destroyDataset(dataset);

  return 0;
}

void usage(char *executable) {
  printf("[USAGE]: %s <#control> <#classify> <#features> <#classes> <#neighbors> [options]\n\n", executable);

  printf("    #control : size of the control universe\n");
  printf("   #classify : number of samples to be classified\n");
  printf("   #features : number of features per each sample\n");
  printf("    #classes : number of different classes in the control universe (smaller than #control)\n");
  printf("  #neighbors : (k) number of closest neighbors needed to classify a sample\n\n");

  printf("[options] \n\n");
  printf("           --run-type, -r : specify run-type plain or multithread (default=plain)\n");
  printf("  --number-of-threads, -t : specify the number of threads and should be used with -r (default=1)\n");
  printf("         --input-file, -f : binary file that includes control samples, classify samples and classes (default=none)\n");
}

double getElapsedTime(struct timeval startTime, struct timeval endTime) {
  return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.00;
}
