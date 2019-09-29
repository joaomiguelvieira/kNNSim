import subprocess

datasets = {"iris"                   : ["100",   "50",      "4",  "3",  "datasets/bin/0_iris.bin",                    "results/accuracy_hw/iris.solution_hw"],
			"wine"                   : ["118",   "60",      "13", "3",  "datasets/bin/1_wine.bin",                    "results/accuracy_hw/wine.solution_hw"],
			"breast_cancer_wisconsin": ["379",   "190",     "30", "2",  "datasets/bin/2_breast_cancer_wisconsin.bin", "results/accuracy_hw/breast_cancer_wisconsin.solution_hw"],
			"car_evaluation"         : ["1152",  "576",     "6",  "4",  "datasets/bin/3_car_evaluation.bin",          "results/accuracy_hw/car_evaluation.solution_hw"],
			"abalone"                : ["2784",  "1393",    "8",  "29", "datasets/bin/4_abalone.bin",                 "results/accuracy_hw/abalone.solution_hw"],
			"bank_marketing"         : ["30140", "15071",   "16", "2",  "datasets/bin/6_bank_marketing.bin",          "results/accuracy_hw/bank_marketing.solution_hw"],
			"poker_hand"             : ["25010", "1000000", "10", "10", "datasets/bin/7_poker_hand.bin",              "results/accuracy_hw/poker_hand.solution_hw"]}

with open("results/accuracy_hw/relative_accuracy.csv", "a") as output_file:
	for dataset in datasets:
		output_file.write(dataset + "," + subprocess.check_output(["./knnsim", datasets[dataset][0], datasets[dataset][1], datasets[dataset][2], datasets[dataset][3], "4", "-f", datasets[dataset][4], "-s", datasets[dataset][5], "-r", "multithread"]).split("kNN accuracy: ")[1].split("%")[0] + "\n")