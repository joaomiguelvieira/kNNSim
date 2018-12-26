# This is a general solution for a makefile

CC=gcc
CFLAGS=-c -ansi -std=c99 -Wall -O3
LDFLAGS=-pthread
SOURCES=KNNSim.c KNNAlgorithm.c Dataset.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=knnsim
HOSTNAME=$(shell hostname)
ifeq ($(MACOS), 1)
    CFLAGS+=-DMACOS
endif

all: $(SOURCES) $(EXECUTABLE) data_sets simulation_multithread

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

data_sets:
	cd datasets && $(MAKE)

simulation_plain:
	mkdir -p sim/plain
	./knnsim 100   50      4   3  4 --run-type plain --input-file datasets/bin/0_iris.bin                                         > sim/plain/0_iris_$(HOSTNAME)_plain.sim
	./knnsim 118   60      13  3  4 --run-type plain --input-file datasets/bin/1_wine.bin                                         > sim/plain/1_wine_$(HOSTNAME)_plain.sim
	./knnsim 379   190     30  2  4 --run-type plain --input-file datasets/bin/2_breast_cancer_wisconsin.bin                      > sim/plain/2_breast_cancer_wisconsin_$(HOSTNAME)_plain.sim
	./knnsim 1152  576     6   4  4 --run-type plain --input-file datasets/bin/3_car_evaluation.bin                               > sim/plain/3_car_evaluation_$(HOSTNAME)_plain.sim
	./knnsim 2784  1393    8   29 4 --run-type plain --input-file datasets/bin/4_abalone.bin                                      > sim/plain/4_abalone_$(HOSTNAME)_plain.sim
	./knnsim 7352  2947    561 6  4 --run-type plain --input-file datasets/bin/5_human_activity_recognition_using_smartphones.bin > sim/plain/5_human_activity_recognition_using_smartphones_$(HOSTNAME)_plain.sim
	./knnsim 30140 15071   16  2  4 --run-type plain --input-file datasets/bin/6_bank_marketing.bin                               > sim/plain/6_bank_marketing_$(HOSTNAME)_plain.sim
	./knnsim 25010 1000000 10  10 4 --run-type plain --input-file datasets/bin/7_poker_hand.bin                                   > sim/plain/7_poker_hand_$(HOSTNAME)_plain.sim

simulation_multithread:
	mkdir -p sim/multithread
	./knnsim 100   50      4   3  4 --run-type multithread --input-file datasets/bin/0_iris.bin                                         > sim/plain/0_iris_$(HOSTNAME)_plain.sim
	./knnsim 118   60      13  3  4 --run-type multithread --input-file datasets/bin/1_wine.bin                                         > sim/plain/1_wine_$(HOSTNAME)_plain.sim
	./knnsim 379   190     30  2  4 --run-type multithread --input-file datasets/bin/2_breast_cancer_wisconsin.bin                      > sim/plain/2_breast_cancer_wisconsin_$(HOSTNAME)_plain.sim
	./knnsim 1152  576     6   4  4 --run-type multithread --input-file datasets/bin/3_car_evaluation.bin                               > sim/plain/3_car_evaluation_$(HOSTNAME)_plain.sim
	./knnsim 2784  1393    8   29 4 --run-type multithread --input-file datasets/bin/4_abalone.bin                                      > sim/plain/4_abalone_$(HOSTNAME)_plain.sim
	./knnsim 7352  2947    561 6  4 --run-type multithread --input-file datasets/bin/5_human_activity_recognition_using_smartphones.bin > sim/plain/5_human_activity_recognition_using_smartphones_$(HOSTNAME)_plain.sim
	./knnsim 30140 15071   16  2  4 --run-type multithread --input-file datasets/bin/6_bank_marketing.bin                               > sim/plain/6_bank_marketing_$(HOSTNAME)_plain.sim
	./knnsim 25010 1000000 10  10 4 --run-type multithread --input-file datasets/bin/7_poker_hand.bin                                   > sim/plain/7_poker_hand_$(HOSTNAME)_plain.sim

clean:
	rm -rf *.o *.~ $(EXECUTABLE) *.gch
	cd datasets && $(MAKE) -w clean