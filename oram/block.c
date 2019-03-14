#include "block.h"
#include "config.h"


Block create_block(int bid, char data[]){


	Block retval;
	
	retval.bid = bid;
	
	int i;
	for(i=0; i <MAX_BLOCK_SIZE; ++i){
		retval.data[i] = data[i];
	
	}
	
	return retval;

}


Block create_dummy_block(){
	Block retval;
	retval.bid = -1;
	
	int i;
	for (i=0; i< MAX_BLOCK_SIZE; ++i){
		retval.data[i] =(char) ' ' + ((int)rand()%(126-33));
	
	}
	retval.data[MAX_BLOCK_SIZE-1] = '\0';
	
	return retval;	
	

}
