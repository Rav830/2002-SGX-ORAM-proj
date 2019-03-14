
#ifndef BLOCK_H
#define BLOCK_H

#include "config.h"


//a block is an array of data with a max size from the config

//data is the content of the block
//bid is the block id and helps indicate where it is in the binary tree
// 
typedef struct{
	int bid;
	char data[MAX_BLOCK_SIZE];

} Block; 

//creates a block with a bid data set to all ones 
Block create_block(int bid, char data[]); 
Block create_dummy_block();

#endif //BLOCK_H
