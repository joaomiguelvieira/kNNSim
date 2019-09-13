#include "KNNSim.h"

int main(int argc, char const *argv[]) {
  int numberControl, numberClassify, numberFeatures, numberClasses, numberNeighbors;
  Dataset *dataset;
  struct timeval startTime, endTime;

  // optional arguments
  RunType        runType           = plain; // type of run
  DistanceMetric distanceMetric    = ssd;   // metric for calculating distances
  int            p                 = 3;     // p in Minkowsky distance
  int            inputFile         = 0;     // optional binary input file
  char           *inputFilename    = NULL;
  int            solutionFile      = 0;     // optional solution file
  char           *solutionFilename = NULL;
  int            numberOfThreads;
  char           hostname[BUFLEN];

  // default number of threads
#ifndef MACOS
  numberOfThreads = get_nprocs();
#else
  size_t sizeOfInt = sizeof(int);
  sysctlbyname("hw.logicalcpu", &numberOfThreads, &sizeOfInt, NULL, 0);
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
    // specify another distance metric that is not sum of square differences
    else if (!strcmp(argv[i], "--distance-metric") || !strcmp(argv[i], "-d")) {
      if     (!strcmp(argv[i + 1], "ssd"      ))  distanceMetric = ssd;
      else if(!strcmp(argv[i + 1], "euclidean"))  distanceMetric = euclidean;
      else if(!strcmp(argv[i + 1], "cosine"))     distanceMetric = cosine;
      else if(!strcmp(argv[i + 1], "chi-square")) distanceMetric = chiSquare;
      else if(!strcmp(argv[i + 1], "minkowsky"))  distanceMetric = minkowsky;
      else if(!strcmp(argv[i + 1], "manhattan"))  distanceMetric = manhattan;
    }
    // specify p in Minkowsky distance
    else if (!strcmp(argv[i], "--minkowsky-p") || !strcmp(argv[i], "-p")) {
      p = atoi(argv[i + 1]);
    }
    else if (!strcmp(argv[i], "--solution-file") || !strcmp(argv[i], "-s")) {
      solutionFile = 1;
      solutionFilename = (char *) argv[i + 1];
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

  // if an input file is provided, use it and check for dataset integrity; otherwise generate the dataset
  if (inputFile == 1) {
    dataset = loadDataset(numberControl, numberClassify, numberFeatures, numberClasses, inputFilename);
    assert(!checkDatasetIntegrity(dataset));
  }
  else
    dataset = randDataset(numberControl, numberClassify, numberFeatures, numberClasses);

  // execute knn algorithm
  gettimeofday(&startTime, NULL);
  knnAlgorithm(runType, dataset, numberNeighbors, distanceMetric, p, numberOfThreads);
  gettimeofday(&endTime, NULL);

  // print summary
  printf("             Hostname: %s\n", hostname);
  printf("             Run type: %s\n", runType == plain ? "plain" : "multithread");
  printf("    Number of threads: %d\n", numberOfThreads);
  printf("      Distance Metric: %s\n", distanceMetric == ssd       ? "ssd (sum of square differences)" : 
                                        distanceMetric == euclidean ? "euclidean"                       :
                                        distanceMetric == cosine    ? "cosine"                          :
                                        distanceMetric == chiSquare ? "chi-square"                      :
                                        distanceMetric == minkowsky ? "minkowsky"                       :
                                                                      "manhattan");
  printf("Minkowsky p parameter: %d\n", p);
  printf("     Elapsed time [s]: %lf\n", getElapsedTime(startTime, endTime));

  // calculate kNN's accuracy
  if (inputFile && solutionFile) {
    printf("         kNN accuracy: %.2f%%\n", calculateAccuracy(dataset, solutionFilename));
  }

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
  printf("           --run-type, -r : run-type plain or multithread (default=plain)\n");
  printf("  --number-of-threads, -t : number of threads and should be used with -r (default=1)\n");
  printf("         --input-file, -f : binary file that includes control samples, classify samples and classes (default=none)\n");

  printf("    --distance-metric, -d : distance metric ssd (sum of square differences), euclidean, cosine, chi-square, minkowsky or manhattan (default=ssd)\n");
  printf("        --minkowsky-p, -p : parameter p of minkowsky distance (default=3)\n");
  printf("      --solution-file, -s : file with the actual classes of the classified samples that allows calculating kNN accuracy (default=none)\n");
}

double getElapsedTime(struct timeval startTime, struct timeval endTime) {
  return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec) / 1000000.00;
}

float calculateAccuracy(Dataset *dataset, char *solutionFilename) {
  float right = 0, wrong = 0;
  int class;
  char line[BUFLEN];

  FILE *solutionFile = fopen(solutionFilename, "r");
  assert(solutionFile != NULL);

  for (int i = 0; i < dataset->numberClassify; i++) {
    assert(fgets(line, BUFLEN, solutionFile) != NULL);
    assert(sscanf(line, "%d", &class) == 1);

    if (class == dataset->classifyClasses[i])
      right++;
    else
      wrong++;
  }

  return right * 100 / (right + wrong);
}
