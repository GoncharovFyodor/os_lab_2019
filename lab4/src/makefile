CC=gcc
CFLAGS=-I.

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

parallel_sum : utils.o sum.o utils.h sum.h
	$(CC) -pthread -o parallel_sum utils.o sum.o parallel_sum.c $(CFLAGS)
	
utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)
    
process_memory :
	$(CC) -o process_memory process_memory.c $(CFLAGS)
    
all : parallel_min_max

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max
