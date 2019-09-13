def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


dataSet = open("wdbc.data", "r")

ctrlSamples = []
testSamples = []
classes = []
solution = []

index = 0

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    if index % 3 == 0:
        if attributes[1] == 'B':
            solution.append(0)
        else:
            solution.append(1)

        del attributes[1]
        del attributes[0]
        testSamples.append(attributes)
    else:
        if attributes[1] == 'B':
            classes.append(0)
        else:
            classes.append(1)

        del attributes[1]
        del attributes[0]
        ctrlSamples.append(attributes)

    attr = len(attributes)

    index = index + 1

dataSet.close()

ctrl = len(ctrlSamples)
test = len(testSamples)

ctrlSamples = [float(i) for i in merge(ctrlSamples)]
testSamples = [float(i) for i in merge(testSamples)]
classes = [int(i) for i in classes]

text = open("breast_cancer_wisconsin.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("breast_cancer_wisconsin.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("2 # classes\n")

config.close()

with open("breast_cancer_wisconsin.solution", "w") as solution_file:
    for item in solution:
        solution_file.write(str(item) + "\n");