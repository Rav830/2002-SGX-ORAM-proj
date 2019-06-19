#include "bucket.hpp"
#include "config.h"
#include "block.hpp"
#include "../printFunc.hpp"
//#include <stdio.h>

Bucket create_dummy_bucket(int doEncrypt){

	Bucket retval;
	
	int i;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		retval.blocks[i] = create_dummy_block(doEncrypt);
		//encrypt_block(&(retval.blocks[i]), 1);
	}
	
	return retval;


}


void print_bucket(Bucket toPrint){
	
	int i, j;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		printf("Block %d:", i);
		printf("\n\t");
		printf("%d ", toPrint.blocks[i].data[0]);
		for(j=1; j<MAX_BLOCK_SIZE; j++){
			printf("%d ", toPrint.blocks[i].data[j]);
		}
		printf("\n");
			
		
	}


}

void print_bucket_no_dummy(Bucket toPrint){
	
	int i, j;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		Block tmp = toPrint.blocks[i];
		decrypt_block(&tmp);
		
		if(tmp.data[0] < HASH_RANGE){
			printf("Block %d:", i);
			printf("\n\t");
			printf("%d ", tmp.data[0]);
			for(j=1; j<MAX_BLOCK_SIZE; j++){
				printf("%c", tmp.data[j]);
			}
			printf("\n");
		}
		/*if(toPrint.blocks[i].data[0] < HASH_RANGE){
			printf("Block %d:", i);
			printf("\n\t");
			printf("%d ", toPrint.blocks[i].data[0]);
			for(j=1; j<MAX_BLOCK_SIZE; j++){
				printf("%c", toPrint.blocks[i].data[j]);
			}
			printf("\n");
		}
		*/
		
	}


}


//@param input: the block to try to assign to the bucket placeIn
//return: -1 for failure (i.e bucket is full of non dummy blocks
//return: 1 for success
int place_block(Block input, Bucket* placeIn){
	
	int i;
	int retval = -1;
	for(i=0; i<MAX_BUCKET_SIZE; ++i){
		if(placeIn->blocks[i].data[0] >= HASH_RANGE){
			placeIn->blocks[i] = input;
			retval = input.data[0];
			break;
		}
	}
	
	return retval;

}
