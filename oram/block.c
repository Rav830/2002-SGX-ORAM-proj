#include "block.h"
#include "config.h"
#include <stdlib.h>

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
	//retval.bid = -1;
	retval.data[0] = (rand()%(255-INIT_STORAGE_ELEMS) )+ INIT_STORAGE_ELEMS ;
	int i;
	for (i=1; i< MAX_BLOCK_SIZE; ++i){
		retval.data[i] =(uint8_t) ' ' + ((int)cnt%(126-33));
		cnt+=1;
	}
	//retval.data[MAX_BLOCK_SIZE-1] = '\0';
	
	return retval;	
	

}
