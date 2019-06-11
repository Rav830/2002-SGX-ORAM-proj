#include "block.hpp"
//#include "../../Include/oramStructs.hpp"
//#include "config.h"
#include "Enclave_t.h"
#include "../printFunc.hpp"


int cnt = 0;

Block create_block(uint8_t bid, uint8_t data[]){


	Block retval;
	
	//retval.bid = bid;
	
	
	retval.data[0] = bid;
	int i;
	for(i=0; i <MAX_BLOCK_SIZE; ++i){
		retval.data[i+1] = data[i];
	
	}
	
	return retval;

}


Block create_dummy_block(){
	Block retval;
	memset(retval.data, 0, MAX_BLOCK_SIZE);
	retval.data[0] = HASH_RANGE;
	/*
	//retval.bid = -1;
	retval.data[0] = (rand()%(255-HASH_RANGE) )+ HASH_RANGE ;
	int i;
	for (i=1; i< MAX_BLOCK_SIZE; ++i){
		retval.data[i] =(uint8_t) ' ' + ((int)cnt%(126-33));
		cnt+=1;
	}
	//retval.data[MAX_BLOCK_SIZE-1] = '\0';
	*/
	return retval;	
	

}


void print_block(Block toPrint){
	int j;
	printf("%d ", toPrint.data[0]);
	for(j=1; j<MAX_BLOCK_SIZE; j++){
		printf("%c", toPrint.data[j]);
	}
}

