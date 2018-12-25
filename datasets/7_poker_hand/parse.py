def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


ctrlSamples = []
testSamples = []
classes = []

dataSet = open("poker-hand-training-true.data", "r")

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    classes.append(attributes[-1])
    del attributes[-1]
    ctrlSamples.append(attributes)

dataSet.close()

dataSet = open("poker-hand-testing.data", "r")

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    del attributes[-1]
    testSamples.append(attributes)

dataSet.close()

ctrl = len(ctrlSamples)
test = len(testSamples)
attr = len(ctrlSamples[0])

ctrlSamples = [float(i) for i in merge(ctrlSamples)]
testSamples = [float(i) for i in merge(testSamples)]
classes = [int(i) for i in classes]

text = open("poker.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("poker.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("10 # classes\n")

config.close()
