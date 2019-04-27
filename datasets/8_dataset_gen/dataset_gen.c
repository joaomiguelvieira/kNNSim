#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
  if(argc < 5) {
    printf("Wrong usage\n");
    exit(-1);
  }

  int numCtrl   = atoi(argv[1]);
  int numTest   = atoi(argv[2]);
  int numCoords = atoi(argv[3]);
  int numClass  = atoi(argv[4]);

  float *dataSet = (float *) malloc((numCtrl + numTest) * numCoords * sizeof(float));

  srand(time(NULL));

  for(int i = 0; i < (numCtrl + numTest) * numCoords; i++) {
    dataSet[i] = (rand() % 1000000);
    dataSet[i] /= 100;
  }

  int *class = (int *) malloc(numCtrl * sizeof(int));

  for(int i = 0; i < numCtrl; i++)
    class[i] = rand() % numClass;

  FILE *bin = fopen((argc > 5) ? argv[5] : "output.bin", "wb");

  fwrite(dataSet, sizeof(float), (numCtrl + numTest) * numCoords, bin);
  fwrite(class, sizeof(int), numCtrl, bin);

  fclose(bin);

  FILE *text = fopen((argc > 5) ? argv[6] : "output.txt", "w");

  fprintf(text, "-TRAINING: %d\n", numCtrl);
  fprintf(text, "-----TEST: %d\n", numTest);
  fprintf(text, "-FEATURES: %d\n", numCoords);
  fprintf(text, "--CLASSES: %d\n\n", numClass);

  for(int i = 0; i < (numCtrl + numTest) * numCoords; i++) {
    fprintf(text, "%.2f ", dataSet[i]);

    if(i % numCoords == numCoords - 1)
      fprintf(text, "\n");
  }

  fprintf(text, "\n\n");

  for(int i = 0; i < numCtrl; i++)
    fprintf(text, "%d ", class[i]);

  return 0;
}