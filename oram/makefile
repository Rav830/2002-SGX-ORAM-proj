OBJS	= main.o block.o bucket.o
SOURCE	= main.c block.c bucket.c
HEADER	= block.h config.h bucket.h
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


clean:
	rm -f -v $(OBJS) $(OUT)

