import argparse
import math
import matplotlib.pyplot as plt


def get_column(data, index):
    column = []

    for element in data:
        column.append(element[index])

    return column


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

# calculate minimum, maximum and average power consumption
min_idle_power = 1000
max_idle_power = 0
avg_idle_power = 0
instant = 0
for power_value in idle_power:
    avg_idle_power = avg_idle_power + power_value[1] * (power_value[0] - instant)
    instant = power_value[0]

    if power_value[1] > max_idle_power:
        max_idle_power = power_value[1]

    if power_value[1] < min_idle_power:
        min_idle_power = power_value[1]

avg_idle_power = avg_idle_power / instant

min_load_power = 1000
max_load_power = 0
avg_load_power = 0
instant = 0
for power_value in load_power:
    avg_load_power = avg_load_power + power_value[1] * (power_value[0] - instant)
    instant = power_value[0]

    if power_value[1] > max_load_power:
        max_load_power = power_value[1]

    if power_value[1] < min_load_power:
        min_load_power = power_value[1]

avg_load_power = avg_load_power / instant

# calculate standard deviation
sd_idle_power = 0
instant = 0
for power_value in idle_power:
    k = power_value[0] - instant
    instant = power_value[0]
    an = power_value[1] - avg_idle_power
    an = k * an * an
    sd_idle_power = sd_idle_power + an

sd_idle_power = math.sqrt(sd_idle_power / instant)

sd_load_power = 0
instant = 0
for power_value in load_power:
    k = power_value[0] - instant
    instant = power_value[0]
    an = power_value[1] - avg_load_power
    an = k * an * an
    sd_load_power = sd_idle_power + an

sd_load_power = math.sqrt(sd_idle_power / instant)

print("""IDLE POWER: minimum : {}
            maximum : {}
            average : {}
            st. dev : {}""".format(min_idle_power, max_idle_power, avg_idle_power, sd_idle_power))
print("""LOAD POWER: minimum : {}
            maximum : {}
            average : {}
            st. dev : {}""".format(min_load_power, max_load_power, avg_load_power, sd_load_power))

x = get_column(load_power, 0)
y = get_column(load_power, 1)

plt.plot(x, y)
plt.xlim(0, max(x))
plt.ylim(0, max(y) + 0.1 * max(y))
plt.xlabel("Time [s]")
plt.ylabel("Instant power [W]")
plt.title("Power consumption on load")
plt.show()
