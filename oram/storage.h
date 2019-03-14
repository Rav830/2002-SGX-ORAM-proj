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
void print_storage(Storage toPrint);

#endif
