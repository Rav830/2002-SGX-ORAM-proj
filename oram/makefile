OBJS	= main.o block.o bucket.o storage.o
SOURCE	= main.c block.c bucket.c storage.c
HEADER	= block.h config.h bucket.h storage.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c 

block.o: block.c
	$(CC) $(FLAGS) block.c 

bucket.o: bucket.c
	$(CC) $(FLAGS) bucket.c 

storage.o: storage.c
	$(CC) $(FLAGS) storage.c 


clean:
	rm -f -v $(OBJS) $(OUT)

