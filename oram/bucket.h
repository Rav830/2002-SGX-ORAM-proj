
#ifndef BUCKET_H
#define BUCKET_H

#include "config.h"
#include "block.h"

//change this from an array of blocks to a 2d array of serialized blocks
typedef struct{
	Block blocks[MAX_BUCKET_SIZE];
}Bucket;


Bucket create_dummy_bucket();
int place_block(Block input, Bucket* placeIn);
void print_bucket(Bucket toPrint);

#endif
