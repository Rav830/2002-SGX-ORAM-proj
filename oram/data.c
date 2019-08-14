#include "data.h"
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


	uint8_t* retval =  (uint8_t*) malloc(64 * sizeof(uint8_t));
	
	
	if(isCust){
		//load the identifier into buffer
		retval[0] = (uint8_t) '+';
		//load in the int to the buffer
		uint32_to_uint8(c->id, (retval+1));
		//load the expire time to buffer
		retval[5] = c->expireTime;
		// now copy the name into the buffer starting at the 8 element
		int i;
		for(i=0; i<25; i++){
			retval[i+8] = (uint8_t)c->name[i];
		}
		
	}
	else{
		//load the identifier into buffer
		retval[0] = (uint8_t) '-';
		//load in the int to the buffer
		uint32_to_uint8(o->id, (retval+1));
		//load the expire time to buffer
		retval[5] = o->expireTime;
		// now copy the name into the buffer starting at the 8 element
		int i;
		for(i=0; i<50; i++){
			retval[i+8] = (uint8_t) o->name[i];
		}
	}	
	return retval;
}


void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust){
	if(isCust){
		//extract the id from the buffer
		c->id = uint8_to_uint32(cereal+1);
		//extract the expiretime
		c->expireTime = cereal[5];
		//copy back the rest of the data 
		int i;
		for(i=0; i<25; i++){
			 c->name[i] = (char)cereal[i+8];
		}	
		
	}else{
		//extract the id from the buffer
		o->id = uint8_to_uint32(cereal+1);
		//extract the expiretime
		o->expireTime = cereal[5];
		//copy back the rest of the data 
		int i;
		for(i=0; i<50; i++){
			 o->name[i] = (char)cereal[i+8];
		}
	}		
}



/*

char* line = malloc(64*sizeof(char));
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
	*/

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust){


	char* retval = malloc(65 * sizeof(uint8_t));

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
	char* line = malloc(65*sizeof(char));
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


