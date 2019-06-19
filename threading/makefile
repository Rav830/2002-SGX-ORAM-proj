OBJS = main.o 
SOURCE = main.c 
HEADER = 
OUT = main 
CC = gcc 
FLAGS = -g -c -Wall 
LFLAGS = -lpthread

all: $(OBJS) 
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) 
	
main.o: main.c 
	$(CC) $(FLAGS) main.c 
	
clean: 
	rm -f -v $(OBJS) $(OUT) 


#made using http://solver.assistedcoding.eu/makefilegen
