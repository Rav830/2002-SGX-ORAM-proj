OBJS	= main.o block.o bucket.o storage.o data.o
SOURCE	= main.c block.c bucket.c storage.c data.c
HEADER	= block.h config.h bucket.h storage.h data.h
OUT	= main
CC	 = gcc
FLAGS	 = -g -c -Wall -Wcomment
LFLAGS	 = -lm
#lm for the math functions of abs, log ...
all: $(OBJS)
	$(CC) -g -fsanitize=address $(OBJS) -o $(OUT) $(LFLAGS)
main.o: main.c config.h
	$(CC) $(FLAGS) main.c 

block.o: block.c config.h
	$(CC) $(FLAGS) block.c 

bucket.o: bucket.c config.h block.o
	$(CC) $(FLAGS) bucket.c 

storage.o: storage.c config.h bucket.o
	$(CC) $(FLAGS) storage.c 

data.o: data.c data.h config.h
	$(CC) $(FLAGS) data.c 

clean:
	rm -f -v $(OBJS) $(OUT)


#made using http://solver.assistedcoding.eu/makefilegen
