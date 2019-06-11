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

int increaseSize(Storage* toIncrease, int newSize);

int idxToParentIndex(int index);
int idxToLeafIndex(int index);
int numLeaves(int numBuckets);
void print_storage(Storage toPrint);
void print_storage_no_dummy(Storage toPrint);


typedef struct{

	Block stash[STASH_SIZE];
	int stashIDX;
	int pmBID[PM_SIZE];
	int pmIDX[PM_SIZE];

}StorageManager;

StorageManager create_manager(void);

int add_bid(StorageManager* lookIn, int bid);
int look_up_bid(StorageManager* lookIn, int bid);
void print_stash(StorageManager* lookIn, int printDummy);


#endif
