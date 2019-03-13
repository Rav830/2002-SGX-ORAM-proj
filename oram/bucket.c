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

