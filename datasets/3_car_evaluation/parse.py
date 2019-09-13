def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


dataSet = open("car.data", "r")

ctrlSamples = []
testSamples = []
classes = []
solution = []

index = 0

for line in dataSet:
    line = line.rstrip("\n")
    attributes = line.split(',')

    # 1st attribute quantification
    if attributes[0] == 'low':
        attributes[0] = '0'
    elif attributes[0] == 'med':
        attributes[0] = '1'
    elif attributes[0] == 'high':
        attributes[0] = '2'
    else:
        attributes[0] = '3'

    # 2nd attribute quantification
    if attributes[1] == 'low':
        attributes[1] = '0'
    elif attributes[1] == 'med':
        attributes[1] = '1'
    elif attributes[1] == 'high':
        attributes[1] = '2'
    else:
        attributes[1] = '3'

    # 3rd attribute quantification
    if attributes[2] == '5more':
        attributes[2] = 5

    # 4th attribute quantification
    if attributes[3] == 'more':
        attributes[3] = 5

    # 5th attribute quantification
    if attributes[4] == 'small':
        attributes[4] = '0'
    elif attributes[4] == 'med':
        attributes[4] = '1'
    else:
        attributes[4] = '3'

    # 6th attribute quantification
    if attributes[5] == 'low':
        attributes[5] = '0'
    elif attributes[5] == 'med':
        attributes[5] = '1'
    else:
        attributes[5] = '3'

    # class quantification
    if attributes[6] == 'unacc':
        attributes[6] = '0'
    elif attributes[6] == 'acc':
        attributes[6] = '1'
    elif attributes[6] == 'good':
        attributes[6] = '2'
    else:
        attributes[6] = '3'

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

text = open("car_evaluation.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("car_evaluation.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("4 # classes\n")

config.close()

with open("car_evaluation.solution", "w") as solution_file:
    for item in solution:
        solution_file.write(str(item) + "\n");