# This is a general solution for a makefile

CC=gcc
CFLAGS=-c -ansi -std=c99 -Wall -mavx2
LDFLAGS=-pthread
SOURCES=KNNSim.c KNNAlgorithm.c Dataset.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=knnsim

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *.~ $(EXECUTABLE) *.gch

backup:
	rm -rf backup.zip
	zip -r -9 backup.zip *.c *.h Makefile iris.bin

run:
	./$(EXECUTABLE) 100 50 4 3 4 iris.bin
