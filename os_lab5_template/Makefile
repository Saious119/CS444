CC = gcc
GLOBAL_FLAGS = -DUNIX -lpthread

all: build

build:
	FLAGS="-O2"
	export FLAGS
	$(MAKE) _all

debug:
	FLAGS="-g"
	export FLAGS
	$(MAKE) _all

_all: main.o thread.o queue.o
	$(CC) $(FLAGS) $(GLOBAL_FLAGS) main.o thread.o queue.o -o ConsumerProducer
	rm *.o

main.o: main.c thread.h queue.h
	$(CC) $(FLAGS) $(GLOBAL_FLAGS) -c main.c -o main.o

thread.o: thread.c thread.h queue.h
	$(CC) $(FLAGS) $(GLOBAL_FLAGS) -c thread.c -o thread.o

queue.o: queue.c queue.h
	$(CC) $(FLAGS) $(GLOBAL_FLAGS) -c queue.c -o queue.o

clean:
	rm ConsumerProducer
