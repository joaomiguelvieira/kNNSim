import argparse

parser = argparse.ArgumentParser();
parser.add_argument("--host", help="name of the host that specify the data files")
args = vars(parser.parse_args())

idle = open(args['host'] + "_idle.csv", "r")
load = open(args['host'] + "_knn.csv", "r")
run = open(args['host'] + "_knn.txt", "r")

time = float(run.read().splitlines()[-1].split(": ")[1])

idle_power = []
for line in idle:
	values = line.rstrip().split(";")
	values[0] = float(values[0])
	values[1] = int(values[1])
	idle_power.append(values)

load_power = []
for line in load:
	values = line.rstrip().split(";")
	values[0] = float(values[0])
	values[1] = int(values[1])

	if values[0] > time:
		break
	else:
		load_power.append(values)
