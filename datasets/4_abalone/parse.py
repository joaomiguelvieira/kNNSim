def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


dataSet = open("abalone.data", "r")

ctrlSamples = []
testSamples = []
classes = []
solution = []

index = 0

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    if attributes[0] == 'M':
        attributes[0] = '0'
    elif attributes[0] == 'F':
        attributes[0] = '1'
    else:
        attributes[0] = '2'

    if index % 3 == 0:
        solution.append(attributes[-1])
        del attributes[-1]
        testSamples.append(attributes)
    else:
        classes.append(attributes[-1])
        del attributes[-1]
        ctrlSamples.append(attributes)

    attr = len(attributes)

    index = index + 1

dataSet.close()

ctrl = len(ctrlSamples)
test = len(testSamples)

ctrlSamples = [float(i) for i in merge(ctrlSamples)]
testSamples = [float(i) for i in merge(testSamples)]
classes = [int(i) for i in classes]

text = open("abalone.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("abalone.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("29 # classes\n")

config.close()

with open("abalone.solution", "w") as solution_file:
    for item in solution:
        solution_file.write(str(item) + "\n");