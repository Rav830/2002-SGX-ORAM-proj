#include "bucket.h"
#include "config.h"
#include "block.h"
#include <stdio.h>

Bucket create_dummy_bucket(){

	Bucket retval;
	
	int i;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		retval.blocks[i] = create_dummy_block();
	}
	
	return retval;


}

void print_bucket(Bucket toPrint){
	
	int i;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		printf("Block %d: %d %s\n", i, toPrint.blocks[i].bid, toPrint.blocks[i].data);
	}


}

//@param input: the block to try to assign to the bucket placeIn
//return: -1 for failure (i.e bucket is full of non dummy blocks
//return: 1 for success
int place_block(Block input, Bucket* placeIn){
	
	int i;
	int retval = -1;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		if(placeIn->blocks[i].bid == -1){
			placeIn->blocks[i] = input;
			retval = input.bid;
			break;
		}
	}
	
	return retval;

}
