OBJS	= main.o block.o bucket.o storage.o data.o
SOURCE	= main.c block.c bucket.c storage.c data.c
HEADER	= block.h config.h bucket.h storage.h data.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c config.h
	$(CC) $(FLAGS) main.c 

block.o: block.c config.h
	$(CC) $(FLAGS) block.c 

bucket.o: bucket.c config.h
	$(CC) $(FLAGS) bucket.c 

storage.o: storage.c config.h
	$(CC) $(FLAGS) storage.c 

data.o: data.c data.h config.h
	$(CC) $(FLAGS) data.c 

clean:
	rm -f -v $(OBJS) $(OUT)


#made using http://solver.assistedcoding.eu/makefilegen
