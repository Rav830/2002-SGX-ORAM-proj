#include "../Include/data.hpp"
#include "config.h"
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
	int i = 0;
	//consume the first line
	FILE* in = fopen(filename, "r");
	char line[1024];
	fgets(line, 1024, in);
	while(fgets(line, 1024, in) && i < numLines){
		char* end;
		retval[i].id = strtol(strtok(line, ","), &end, 10);
		strcpy(retval[i].name, strtok(NULL, ","));
		retval[i].expireTime = strtol(strtok(NULL, ","), &end, 10);
		
		i++;
	}
	
	fclose(in);
}

void readOrder(char* filename, int numLines, Order* retval){
	//Customer retval[numLines];
	int i = 0;
	//consume the first line
	FILE* in = fopen(filename, "r");
	
	char line[1024];
	fgets(line, 1024, in);
	char* end;
	while(fgets(line, 1024, in) && i < numLines){
		retval[i].id = strtol(strtok(line, ","), &end, 10);
		strcpy(retval[i].name, strtok(NULL, ","));
		retval[i].expireTime = strtol(strtok(NULL, ","), &end, 10);
		i++;
	}
	fclose(in);
}

uint8_t* serialize(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc(64 * sizeof(uint8_t));

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
	char* line = (char*) malloc(64*sizeof(char));
	char* end;
	memcpy(line, (char*)cereal, 64);
	
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

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc(65 * sizeof(uint8_t));

	if(isCust){
		char* tmp = custToStr(*c);
		sprintf((retval+1), "+,%s", tmp);
		
		retval[0] = hash((uint8_t*)retval);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		sprintf( (retval+1), "-,%s", tmp);
		retval[0] = hash((uint8_t*)retval);
		free(tmp);
	}	
	return (uint8_t*)retval;
}

void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = (char*) malloc(65*sizeof(char));
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


//determines if the serialized data is a customer or not
int isCust(uint8_t* cereal){
	
	if(cereal[0] == (uint8_t)'+'){
		return 1;
	}
	if(cereal[0] == (uint8_t)'-'){
		return 0;
	}


	printf("Array is not a Customer or order\n");
	return -1;

}

int hash(uint8_t* str)
{
    int hash = 5381;
    int i, commaFlag = 0;
	
	for(i = 0; i<65; i++){
		if(str[i] == ((int)',')){
			commaFlag++;
		}
		else{
			//compute hash
			if(commaFlag == 1){
				hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */	
			}
		}
		if(commaFlag == 2){
			break;
		}
		
	}

    return (int)(hash%HASH_RANGE);
}


