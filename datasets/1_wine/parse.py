def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


dataSet = open("wine.data", "r")

ctrlSamples = []
testSamples = []
classes = []
solution = []

index = 0

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    if index % 3 == 0:
        solution.append(int(attributes[0]) - 1)
        del attributes[0]
        testSamples.append(attributes)
    else:
        classes.append(int(attributes[0]) - 1)
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

text = open("wine.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("wine.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("3 # classes\n")

config.close()

with open("wine.solution", "w") as solution_file:
    for item in solution:
        solution_file.write(str(item) + "\n");