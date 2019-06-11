
#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "printFunc.hpp"

char* custToStr(Customer c){
	char* retval = (char*) malloc(64*sizeof(char));
	
	snprintf(retval, 64, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}


char* orderToStr(Order c){
	char* retval = (char*) malloc(64*sizeof(char));
	
	snprintf(retval, 64, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}

uint8_t* serialize(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc(64 * sizeof(uint8_t));
	memset(retval, 0, 64);
	/*int i;
	printf("Serializing pre save \n");
	for(int i=0; i<64; i++){
		printf("%d ", retval[i]);
	}
	printf("\n");
*/
	if(isCust){
		char* tmp = custToStr(*c);
		snprintf(retval, 64, "+,%s", tmp);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		snprintf(retval, 64, "-,%s", tmp);
		
		free(tmp);
	}
	
/*		printf("Serializing post save \n");
	for(int i=0; i<64; i++){
		printf("%d ", retval[i]);
	}
	printf("\n");
*/		
	return (uint8_t*)retval;
}

void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = (char*) malloc(64*sizeof(char));
	char* end;
	memcpy(line, (char*)cereal, 64);
	
	strtok(line,",");

	if(isCust){

		
		(*c).id = strtol(strtok(NULL, ","), &end, 10);
		strncpy((*c).name, strtok(NULL, ","), 25);
		(*c).expireTime = strtol(strtok(NULL, ","), &end, 10);
		
		
	}
	else{
		(*o).id = strtol(strtok(NULL, ","), &end, 10);
		strncpy((*o).name, strtok(NULL, ","),50);
		(*o).expireTime = strtol(strtok(NULL, ","), &end, 10);
	}	
	
	free(line);
	//free(end);
}

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc(65 * sizeof(uint8_t));

	if(isCust){
		char* tmp = custToStr(*c);
		snprintf((retval+1), 64, "+,%s", tmp);
		
		retval[0] = hash((uint8_t*)retval);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		snprintf( (retval+1), 64, "-,%s", tmp);
		retval[0] = hash((uint8_t*)retval);
		free(tmp);
	}	
	return (uint8_t*)retval;
}

void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = (char*) malloc(65*sizeof(char));
	char* end;
	strncpy(line, (char*)cereal, 65);
	
	strtok(line,",");

	if(isCust){

		
		(*c).id = strtol(strtok(NULL, ","), &end, 10);
		strncpy((*c).name, strtok(NULL, ","), 25);
		(*c).expireTime = strtol(strtok(NULL, ","), &end, 10);
		
		
	}
	else{
		(*o).id = strtol(strtok(NULL, ","), &end, 10);
		strncpy((*o).name, strtok(NULL, ","), 50);
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
    int hash = 6969;
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


