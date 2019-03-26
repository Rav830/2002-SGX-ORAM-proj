#ifndef STORAGE_H
#define STORAGE_H

#include "config.h"
#include "block.h"
#include "bucket.h"

typedef struct{
	int numBuckets;
	Bucket allBuckets[INIT_STORAGE_SIZE];
} Storage;



Storage create_storage(void);
void get_buckets(Storage collectFrom, int index, Bucket* retval);
void set_buckets(Storage* toPlace, int index, Bucket* buckets);
int place_block_in_storage(Storage* toPlace, Block data, int index);

int idxToParentIndex(int index);
int idxToLeafIndex(int index);
int numLeaves(int numBuckets);
void print_storage(Storage toPrint);

#endif