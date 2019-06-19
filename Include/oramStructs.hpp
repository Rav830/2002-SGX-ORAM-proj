#ifndef ORAMSTRUCTS_H
#define ORAMSTRUCTS_H

#include "config.h"
#include <stdint.h>
//a block is an array of data with a max size from the config

//data is the content of the block

typedef struct{
	//int bid;
	uint8_t data[MAX_BLOCK_SIZE];
	uint8_t init_vec[12];
	uint8_t mac[16];

} Block; 

//change this from an array of blocks to a 2d array of serialized blocks
typedef struct{
	Block blocks[MAX_BUCKET_SIZE];
}Bucket;

typedef struct{
	int numBuckets;
	int height;
	int leaves;
	Bucket allBuckets[INIT_STORAGE_SIZE];//[(int)(pow(2, log(INIT_STORAGE_ELEMS)/log(2)+1)-1)];
} Storage;

#endif
