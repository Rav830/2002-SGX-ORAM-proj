
#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "printFunc.hpp"

char* custToStr(Customer c){
	char* retval = (char*) malloc(TUPLE_SIZE*sizeof(char));
	
	snprintf(retval, TUPLE_SIZE, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}


char* orderToStr(Order c){
	char* retval = (char*) malloc(TUPLE_SIZE*sizeof(char));
	
	snprintf(retval, TUPLE_SIZE, "%d,%s,%d", c.id, c.name, c.expireTime);
	
	return retval;

}


void uint32_to_uint8(int from, uint8_t* to){
	uint8_t* tmp = (uint8_t*)&from;
	to[0] = tmp[0];
	to[1] = tmp[1];
	to[2] = tmp[2];
	to[3] = tmp[3];
}

int uint8_to_uint32(uint8_t* in){
	int retval = in[0] | (in[1] << 8) | (in[2] << 16) | (in[3] << 24);
	return retval; 
}

uint8_t* serialize(Customer* c, Order* o, int isCust){


	uint8_t* retval =  (uint8_t*) malloc(TUPLE_SIZE * sizeof(uint8_t));
	memset(retval, 0, TUPLE_SIZE);
	
	if(isCust){
		//load the identifier into buffer
		retval[0] = (uint8_t) '+';
		//load in the int to the buffer
		uint32_to_uint8(c->id, (retval+1));
		//load the expire time to buffer
		uint32_to_uint8(c->expireTime, (retval+5));
		//retval[5] = c->expireTime;
		// now copy the name into the buffer starting at the 8 element
		int i;
		for(i=0; i<25; i++){
			retval[i+9] = (uint8_t)c->name[i];
		}
		
	}
	else{
		//load the identifier into buffer
		retval[0] = (uint8_t) '-';
		//load in the int to the buffer
		uint32_to_uint8(o->id, (retval+1));
		//load the expire time to buffer
		uint32_to_uint8(o->expireTime, (retval+5));
		//retval[5] = c->expireTime;
		// now copy the name into the buffer starting at the 8 element
		int i;
		for(i=0; i<50; i++){
			retval[i+9] = (uint8_t) o->name[i];
		}
	}	
	return retval;
}


void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust){
	//printf("is cust: %d, first char: %c\n", isCust, cereal[0]);
	if(isCust){
		
		//extract the id from the buffer
		c->id = uint8_to_uint32(cereal+1);
		//extract the expiretime
		c->expireTime = uint8_to_uint32(cereal+5);//cereal[5];
		//copy back the rest of the data 
		int i;
		for(i=0; i<25; i++){
			 c->name[i] = (char)cereal[i+9];
		}	
		
	}else{
		//extract the id from the buffer
		o->id = uint8_to_uint32(cereal+1);
		//extract the expiretime
		o->expireTime = uint8_to_uint32(cereal+5);//cereal[5];
		//copy back the rest of the data 
		int i;
		for(i=0; i<50; i++){
			 o->name[i] = (char)cereal[i+9];
		}
	}		
}



/*uint8_t* serialize(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc(TUPLE_SIZE * sizeof(uint8_t));
	memset(retval, 0, TUPLE_SIZE);
	int i;
	printf("Serializing pre save \n");
	for(int i=0; i<64; i++){
		printf("%d ", retval[i]);
	}
	printf("\n");

	if(isCust){
		char* tmp = custToStr(*c);
		snprintf(retval, TUPLE_SIZE, "+,%s", tmp);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		snprintf(retval, TUPLE_SIZE, "-,%s", tmp);
		
		free(tmp);
	}
	
		printf("Serializing post save \n");
	for(int i=0; i<64; i++){
		printf("%d ", retval[i]);
	}
	printf("\n");
		
	return (uint8_t*)retval;
}

void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = (char*) malloc(TUPLE_SIZE*sizeof(char));
	char* end;
	memcpy(line, (char*)cereal, TUPLE_SIZE);
	
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
}*/

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust){


	char* retval = (char*) malloc( (TUPLE_SIZE+1) * sizeof(uint8_t));

	if(isCust){
		char* tmp = custToStr(*c);
		snprintf((retval+1), TUPLE_SIZE, "+,%s", tmp);
		
		retval[0] = hash((uint8_t*)retval);
		
		free(tmp);
		
	}
	else{
		char* tmp = orderToStr(*o);
		snprintf( (retval+1), TUPLE_SIZE, "-,%s", tmp);
		retval[0] = hash((uint8_t*)retval);
		free(tmp);
	}	
	return (uint8_t*)retval;
}

void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust){
	char* line = (char*) malloc( (TUPLE_SIZE+1)*sizeof(char));
	char* end;
	strncpy(line, (char*)cereal, (TUPLE_SIZE+1));
	
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
uint8_t hash(uint8_t* str)
{
    uint32_t hash = 6969;
    int i;
	for(i = 1; i<5; i++){
		//compute hash
		hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */	
	}
    return (uint8_t)(hash%HASH_RANGE);
}


