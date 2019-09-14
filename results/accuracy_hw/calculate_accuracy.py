from itertools import izip

datasets = {"iris"                   : ["iris.solution_hw", "../../datasets/0_iris/iris.solution"],
            "wine"                   : ["wine.solution_hw", "../../datasets/1_wine/wine.solution"],
            "breast_cancer_wisconsin": ["breast_cancer_wisconsin.solution_hw", "../../datasets/2_breast_cancer_wisconsin/breast_cancer_wisconsin.solution"],
            "car_evaluation"         : ["car_evaluation.solution_hw", "../../datasets/3_car_evaluation/car_evaluation.solution"],
            "abalone"                : ["abalone.solution_hw", "../../datasets/4_abalone/abalone.solution"],
            "bank_marketing"         : ["bank_marketing.solution_hw", "../../datasets/6_bank_marketing/bank.solution"],
            "poker_hand"             : ["poker_hand.solution_hw", "../../datasets/7_poker_hand/poker.solution"]}

for dataset in datasets:
	right = 0.0
	wrong = 0.0
	for line1, line2 in izip(open(datasets[dataset][0]), open(datasets[dataset][1])):
		if line1 == line2:
			right += 1
		else:
			wrong += 1
	print dataset + "," + str(right / (right + wrong))