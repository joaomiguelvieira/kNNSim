#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int main(int argc, char const *argv[])
{
	// load values from file
	FILE *cfg = fopen(argv[1], "r");
	FILE *parse = fopen(argv[2], "r");

	assert(cfg != NULL && parse != NULL);

	int ctrl, test, features, classes;

	char line[100];
	fgets(line, 100, cfg); sscanf(line, "%d", &ctrl);
	fgets(line, 100, cfg); sscanf(line, "%d", &test);
	fgets(line, 100, cfg); sscanf(line, "%d", &features);
	fgets(line, 100, cfg); sscanf(line, "%d", &classes);

	float *dataSet = (float *) malloc((ctrl + test) * features * sizeof(float));
	int *classesCtrl = (int *) malloc(ctrl * sizeof(int));

	for (int i = 0; i < (ctrl + test) * features; ++i)
	{
		assert(fgets(line, 100, parse) != NULL);
		sscanf(line, "%f", &dataSet[i]);
	}

	for (int i = 0; i < ctrl; ++i)
	{
		assert(fgets(line, 100, parse) != NULL);
		sscanf(line, "%d", &classesCtrl[i]);
	}

	fclose(cfg);
	fclose(parse);

	// check dataset integrity
	for (int i = 0; i < ctrl; ++i)
	{
		assert(classesCtrl[i] >= 0 && classesCtrl[i] < classes);
	}

	// write binary file
	FILE *bin = fopen((argc > 3) ? argv[3] : "dataset.bin", "wb");

	fwrite(dataSet, sizeof(float), (ctrl + test) * features, bin);
	fwrite(classesCtrl, sizeof(int), ctrl, bin);

	fclose(bin);

	free(dataSet);
	free(classesCtrl);

	return 0;
}
