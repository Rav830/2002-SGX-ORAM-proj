
#ifndef BUCKET_H
#define BUCKET_H

#include "config.h"
#include "block.h"

typedef struct{
	Block blocks[MAX_BUCKET_SIZE];
}Bucket;


Bucket create_dummy_bucket();
void assign_block(int bucketIndex, Block input);
void print_bucket(Bucket toPrint);

#endif
