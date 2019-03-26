#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>

char* custToStr(Customer c){
	char* retval = (char*) malloc(64*sizeof(char));
	
	sprintf(retval, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}


char* orderToStr(Order c){
	char* retval = (char*) malloc(64*sizeof(char));
	
	sprintf(retval, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}


void readCust(char* filename, int numLines, Customer* retval){
	//Customer retval[numLines];
	
	int i = -1;
	//consume the first line
	FILE* in = fopen(filename, "r");
	char line[1024];
	
	while(fgets(line, 1024, in)){
		if(i>=0){
			char* end;
			
			retval[i].id = strtol(strtok(line, ","), &end, 10);
			strcpy(retval[i].name, strtok(NULL, ","));
			retval[i].expireTime = strtol(strtok(NULL, ","), &end, 10);
			
		
		}
		i++;
	}
}

void readOrder(char* filename, int numLines, Order* retval){
	//Customer retval[numLines];
	
	int i = -1;
	//consume the first line
	FILE* in = fopen(filename, "r");
	char line[1024];
	char* end;
	while(fgets(line, 1024, in)){
		if(i>=0){
			retval[i].id = strtol(strtok(line, ","), &end, 10);
			strcpy(retval[i].name, strtok(NULL, ","));
			retval[i].expireTime = strtol(strtok(NULL, ","), &end, 10);
		}
		i++;
	}
	
	//free(end);
}

uint8_t* serialize(Customer* c, Order* o, int isCust){


	char* retval = malloc(64 * sizeof(uint8_t));

	if(isCust){
		char* tmp = custToStr(*c);
		sprintf(retval, "+,%s", tmp);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		sprintf(retval, "-,%s", tmp);
		
		free(tmp);
	}	
	return (uint8_t*)retval;
}

void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = malloc(64*sizeof(char));
	char* end;
	strcpy(line, (char*)cereal);
	
	strtok(line,",");

	if(isCust){

		
		(*c).id = strtol(strtok(NULL, ","), &end, 10);
		strcpy((*c).name, strtok(NULL, ","));
		(*c).expireTime = strtol(strtok(NULL, ","), &end, 10);
		
		
	}
	else{
		(*o).id = strtol(strtok(NULL, ","), &end, 10);
		strcpy((*o).name, strtok(NULL, ","));
		(*o).expireTime = strtol(strtok(NULL, ","), &end, 10);
	}	
	
	free(line);
	//free(end);
}
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


