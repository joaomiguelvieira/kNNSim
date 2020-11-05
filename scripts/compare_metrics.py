import subprocess

datasets = {"iris"                   : ["100",   "50",      "4",  "3",  "../datasets/bin/0_iris.bin",                    "../datasets/0_iris/iris.solution"],
			"wine"                   : ["118",   "60",      "13", "3",  "../datasets/bin/1_wine.bin",                    "../datasets/1_wine/wine.solution"],
			"breast_cancer_wisconsin": ["379",   "190",     "30", "2",  "../datasets/bin/2_breast_cancer_wisconsin.bin", "../datasets/2_breast_cancer_wisconsin/breast_cancer_wisconsin.solution"],
			"car_evaluation"         : ["1152",  "576",     "6",  "4",  "../datasets/bin/3_car_evaluation.bin",          "../datasets/3_car_evaluation/car_evaluation.solution"],
			"abalone"                : ["2784",  "1393",    "8",  "29", "../datasets/bin/4_abalone.bin",                 "../datasets/4_abalone/abalone.solution"],
			"bank_marketing"         : ["30140", "15071",   "16", "2",  "../datasets/bin/6_bank_marketing.bin",          "../datasets/6_bank_marketing/bank.solution"],
			"poker_hand"             : ["25010", "1000000", "10", "10", "../datasets/bin/7_poker_hand.bin",              "../datasets/7_poker_hand/poker.solution"]}

metrics  = ["ssd", "euclidean", "cosine", "chi-square", "minkowsky", "manhattan"]

k_list = range(1, 22)

for metric in metrics:
	with open("../results/metrics/" + metric + ".csv", "a") as output_file:
		output_file.write("k,")

		for k in k_list:
			output_file.write(str(k) + ",")

		output_file.write("\n")

		print "Metric: " + metric
		for dataset in datasets:
			output_file.write(dataset + ",")

			print "\tDataset: " + dataset
			for k in k_list:
				print "\t\tk = " + str(k)
				output_file.write(subprocess.check_output(["../knnsim", datasets[dataset][0], datasets[dataset][1], datasets[dataset][2], datasets[dataset][3], str(k), "-f", datasets[dataset][4], "-s", datasets[dataset][5], "-d", metric, "-r", "multithread"]).split("accuracy:\033[0m ")[1].split("%")[0] + ",")

			output_file.write("\n")