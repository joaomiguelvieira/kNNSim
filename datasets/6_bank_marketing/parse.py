import re

def merge(list_of_lists):
    merged_list = []

    for list in list_of_lists:
        for item in list:
            merged_list.append(item)

    return merged_list


job = {
    "admin.": "0",
    "unknown": "1",
    "unemployed": "2",
    "management": "3",
    "housemaid": "4",
    "entrepreneur": "5",
    "student": "6",
    "blue-collar": "7",
    "self-employed": "8",
    "retired": "9",
    "technician": "10",
    "services": "11"
}

marital = {
    "married": "0",
    "divorced": "1",
    "single": "2"
}

education = {
    "unknown": "0",
    "secondary": "1",
    "primary": "2",
    "tertiary": "3"
}

binary = {
    "no": "0",
    "yes": "1"
}

contact = {
    "unknown": "0",
    "telephone": "1",
    "cellular": "2"
}

month = {
    "jan": "0",
    "feb": "1",
    "mar": "2",
    "apr": "3",
    "may": "4",
    "jun": "5",
    "jul": "6",
    "aug": "7",
    "sep": "8",
    "oct": "9",
    "nov": "10",
    "dec": "11"
}

poutcome = {
    "unknown": "0",
    "other": "1",
    "failure": "2",
    "success": "3"
}

dataSet = open("bank-full.csv", "r")

ctrlSamples = []
testSamples = []
classes = []

index = 0

for line in dataSet:
    line = line.rstrip("\n")
    line = re.sub('"', "", line)
    attributes = line.split(';')

    attributes[1] = job[attributes[1]]
    attributes[2] = marital[attributes[2]]
    attributes[3] = education[attributes[3]]
    attributes[4] = binary[attributes[4]]
    attributes[6] = binary[attributes[6]]
    attributes[7] = binary[attributes[7]]
    attributes[8] = contact[attributes[8]]
    attributes[10] = month[attributes[10]]
    attributes[15] = poutcome[attributes[15]]
    attributes[16] = binary[attributes[16]]

    if index % 3 == 0:
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

text = open("bank.parse", "w")

for item in merge([ctrlSamples, testSamples, classes]):
    text.write(str(item) + '\n')

text.close()

config = open("bank.cfg", "w")

config.write(str(ctrl) + " # ctrl samples\n")
config.write(str(test) + " # test samples\n")
config.write(str(attr) + " # features\n")
config.write("2 # classes\n")

config.close()
