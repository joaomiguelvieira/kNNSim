all: bin

parse:
	cd 0_iris && $(MAKE) -w
	cd 1_wine && $(MAKE) -w
	cd 2_breast_cancer_wisconsin && $(MAKE) -w
	cd 3_car_evaluation && $(MAKE) -w
	cd 4_abalone && $(MAKE) -w
	cd 5_human_activity_recognition_using_smartphones && $(MAKE) -w
	cd 6_bank_marketing && $(MAKE) -w
	cd 7_poker_hand && $(MAKE) -w

parse2bin:
	gcc --std=c99 parse2bin.c -o parse2bin

bin: parse parse2bin
	mkdir bin
	./parse2bin 0_iris/iris.cfg 0_iris/iris.parse bin/0_iris.bin
	./parse2bin 1_wine/wine.cfg 1_wine/wine.parse bin/1_wine.bin
	./parse2bin 2_breast_cancer_wisconsin/breast_cancer_wisconsin.cfg 2_breast_cancer_wisconsin/breast_cancer_wisconsin.parse bin/2_breast_cancer_wisconsin.bin
	./parse2bin 3_car_evaluation/car_evaluation.cfg 3_car_evaluation/car_evaluation.parse bin/3_car_evaluation.bin
	./parse2bin 4_abalone/abalone.cfg 4_abalone/abalone.parse bin/4_abalone.bin
	./parse2bin 5_human_activity_recognition_using_smartphones/human_activity_recognition_using_smartphones.cfg 5_human_activity_recognition_using_smartphones/human_activity_recognition_using_smartphones.parse bin/5_human_activity_recognition_using_smartphones.bin
	./parse2bin 6_bank_marketing/bank.cfg 6_bank_marketing/bank.parse bin/6_bank_marketing.bin
	./parse2bin 7_poker_hand/poker.cfg 7_poker_hand/poker.parse bin/7_poker_hand.bin

clean:
	cd 0_iris && $(MAKE) -w clean
	cd 1_wine && $(MAKE) -w clean
	cd 2_breast_cancer_wisconsin && $(MAKE) -w clean
	cd 3_car_evaluation && $(MAKE) -w clean
	cd 4_abalone && $(MAKE) -w clean
	cd 5_human_activity_recognition_using_smartphones && $(MAKE) -w clean
	cd 6_bank_marketing && $(MAKE) -w clean
	cd 7_poker_hand && $(MAKE) -w clean
	rm -rf parse2bin bin
	