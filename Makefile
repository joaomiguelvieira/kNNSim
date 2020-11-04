HOSTNAME=$(shell hostname)

CC=gcc

CFLAGS=-std=c99 -O3
ifeq ($(CUDA), 1)
	CFLAGS+=-I$(CUDA_INSTALL_DIR)/include/ -DCUDA
endif
ifeq ($(MACOS), 1)
    CFLAGS+=-DMACOS
endif

LDFLAGS=-pthread -lm
ifeq ($(CUDA), 1)
	LDFLAGS+=-L$(CUDA_INSTALL_DIR)/lib64 -lcuda -lcudart
endif

# list of sources
SRC_C=KNNSim.c KNNDataset.c KNNClassifier.c KNNAlgorithm.c DistanceMetrics.c Common.c Parser.c SinglyLinkedList.c Argument.c
SRC_CUDA=

OBJ=$(SRC_C:.c=.o)
ifeq ($(CUDA), 1)
	OBJ+=$(SRC_CUDA:.cu=.o)
endif

# name of executable
EXE=knnsim

all: $(SRC_C) $(SRC_CUDA) $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

%.o: %.cu
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(EXE) *.gch *.~

data_sets:
	cd datasets && $(MAKE) -w

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
	./knnsim 100   50      4   3  4 --run-type multithread --input-file datasets/bin/0_iris.bin                                         > sim/multithread/0_iris_$(HOSTNAME)_multithread.sim
	./knnsim 118   60      13  3  4 --run-type multithread --input-file datasets/bin/1_wine.bin                                         > sim/multithread/1_wine_$(HOSTNAME)_multithread.sim
	./knnsim 379   190     30  2  4 --run-type multithread --input-file datasets/bin/2_breast_cancer_wisconsin.bin                      > sim/multithread/2_breast_cancer_wisconsin_$(HOSTNAME)_multithread.sim
	./knnsim 1152  576     6   4  4 --run-type multithread --input-file datasets/bin/3_car_evaluation.bin                               > sim/multithread/3_car_evaluation_$(HOSTNAME)_multithread.sim
	./knnsim 2784  1393    8   29 4 --run-type multithread --input-file datasets/bin/4_abalone.bin                                      > sim/multithread/4_abalone_$(HOSTNAME)_multithread.sim
	./knnsim 7352  2947    561 6  4 --run-type multithread --input-file datasets/bin/5_human_activity_recognition_using_smartphones.bin > sim/multithread/5_human_activity_recognition_using_smartphones_$(HOSTNAME)_multithread.sim
	./knnsim 30140 15071   16  2  4 --run-type multithread --input-file datasets/bin/6_bank_marketing.bin                               > sim/multithread/6_bank_marketing_$(HOSTNAME)_multithread.sim
	./knnsim 25010 1000000 10  10 4 --run-type multithread --input-file datasets/bin/7_poker_hand.bin                                   > sim/multithread/7_poker_hand_$(HOSTNAME)_multithread.sim
