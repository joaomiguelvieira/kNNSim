#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdint.h>

const char *labels[] = {"agricultural", "airplane", "baseballdiamond", "beach", "buildings", "chaparral", "denseresidential", "forest", "freeway", "golfcourse", "harbor", "intersection", "mediumresidential", "mobilehomepark", "overpass", "parkinglot", "river", "runway", "sparseresidential", "storagetanks", "tenniscourt"};

#define NUM_CLASSES     21
#define CTRL_PER_CLASS  5
#define TEST_PER_CLASS  1
#define FEATURES        196608 // 256 * 256 * 3
#define MAX_COLOR_VAL   255.0
#define MAX_STREAM_SIZE 268435456 // 2^26 * 4 bytes

	
int main(int argc, char const *argv[]) {
	// allocate memory for control set
	float **controlSet    = (float **) malloc(CTRL_PER_CLASS * NUM_CLASSES *            sizeof(float *)); assert(controlSet);
	        controlSet[0] = (float *)  malloc(CTRL_PER_CLASS * NUM_CLASSES * FEATURES * sizeof(float)  ); assert(controlSet[0]);
	for (int i = 1; i < CTRL_PER_CLASS * NUM_CLASSES; i++)
		controlSet[i] = controlSet[i - 1] + FEATURES;

	// allocate memory for test_PER_CLASS set
	float **testSet    = (float **) malloc(TEST_PER_CLASS * NUM_CLASSES *            sizeof(float *)); assert(testSet);
	        testSet[0] = (float *)  malloc(TEST_PER_CLASS * NUM_CLASSES * FEATURES * sizeof(float)  ); assert(testSet[0]);
	for (int i = 1; i < TEST_PER_CLASS * NUM_CLASSES; i++)
		testSet[i] = testSet[i - 1] + FEATURES;

	// allocate memory for classes of control set
	int *classesCtrl = (int *) malloc(CTRL_PER_CLASS * NUM_CLASSES * sizeof(int));
	
	// for all folders containing images
	for(int i = 0; i < NUM_CLASSES; i++) {
		// for all images within a folder
		for(int j = 0; j < (CTRL_PER_CLASS + TEST_PER_CLASS); j++) {
			// generate image name
			char buf[80];
			sprintf(buf, "./Images/%s/%s%02d.tif.ppm", labels[i], labels[i], j);
			
			// open image
			FILE *fp = fopen(buf, "rb");
			assert(fp);
			
			// seek ppm header
			fseek(fp, 15, SEEK_SET);

			// read image and load features
			for(int k = 0; k < FEATURES; k++) {
				unsigned char pixel;
				fread(&pixel, sizeof(unsigned char), 1, fp);

				// its a control image
				if (j < CTRL_PER_CLASS)
					controlSet[i * CTRL_PER_CLASS + j][k] = (float) pixel / MAX_COLOR_VAL;
				// its a test image
				else
					testSet[i * TEST_PER_CLASS + j - CTRL_PER_CLASS][k] = (float) pixel / MAX_COLOR_VAL;
			}

			fclose(fp);

			if(j < CTRL_PER_CLASS)
				classesCtrl[i * CTRL_PER_CLASS + j] = i;
		}
	}

	// write binary file
	FILE *bin = fopen((argc > 1) ? argv[1] : "dataset.bin", "wb");

	fwrite(controlSet[0], sizeof(float), CTRL_PER_CLASS * NUM_CLASSES * FEATURES, bin);
	fwrite(testSet[0],    sizeof(float), TEST_PER_CLASS * NUM_CLASSES * FEATURES, bin);
	fwrite(classesCtrl,   sizeof(int),   CTRL_PER_CLASS * NUM_CLASSES,            bin);

	fclose(bin);

	// free memory structures
	free(controlSet[0]);
	free(controlSet);
	free(testSet[0]);
	free(testSet);
	free(classesCtrl);

	printf("        CLASSES: %d\n", NUM_CLASSES);
	printf("CONTROL SAMPLES: %d\n", CTRL_PER_CLASS * NUM_CLASSES);
	printf("   TEST SAMPLES: %d\n", TEST_PER_CLASS * NUM_CLASSES);
	printf("       FEATURES: %d\n", FEATURES);

	uint64_t stream_size = CTRL_PER_CLASS * NUM_CLASSES * FEATURES * sizeof(float);
	uint64_t stream_perc = stream_size * 100 / MAX_STREAM_SIZE;

	printf("Control set size is %llu percent of maximum (%llu bytes out of %d)\n", stream_perc, stream_size, MAX_STREAM_SIZE);

	return 0;
}
