#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"
#include "block.h"
#include "bucket.h"
//#include <math.h>

typedef struct{
	int numBuckets;
	int height;
	int leaves;
	Bucket allBuckets[INIT_STORAGE_SIZE];//[(int)(pow(2, log(INIT_STORAGE_ELEMS)/log(2)+1)-1)];
} Storage;



Storage create_storage(void);
void get_buckets(Storage* collectFrom, int index, Bucket* retval);
void set_buckets(Storage* toPlace, int index, Bucket* buckets);
int place_block_in_storage(Storage* toPlace, Block data, int index);

int idxToParentIndex(int index);
int idxToLeafIndex(int index);
int numLeaves(int numBuckets);
void print_storage(Storage toPrint);

#endif
