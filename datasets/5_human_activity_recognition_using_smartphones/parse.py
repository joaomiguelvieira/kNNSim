def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


ctrlSamples = []
testSamples = []
classes = []

dataSet = open("X_train.txt", "r")

for line in dataSet:
    line = line.rstrip("\n")
    line = line.rstrip("\r")
    attributes = line.split(' ')
    attributes = filter(lambda a: a != '', attributes)
    ctrlSamples.append(attributes)

dataSet.close()

dataSet = open("y_train.txt", "r")

for line in dataSet:
    line = line.rstrip("\n")
    line = line.rstrip("\r")
    classes.append(str(int(line) - 1))

dataSet.close()

dataSet = open("X_test.txt", "r")

for line in dataSet:
    line = line.rstrip("\n")
    line = line.rstrip("\r")
    attributes = line.split(' ')
    attributes = filter(lambda a: a != '', attributes)
    testSamples.append(attributes)

dataSet.close()

ctrl = len(ctrlSamples)
test = len(testSamples)
attr = len(ctrlSamples[0])

ctrlSamples = [float(i) for i in merge(ctrlSamples)]
testSamples = [float(i) for i in merge(testSamples)]
classes = [int(i) for i in classes]

text = open("human_activity_recognition_using_smartphones.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("human_activity_recognition_using_smartphones.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("6 # classes\n")

config.close()
