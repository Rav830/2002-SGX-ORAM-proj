#include "storage.h"
#include "bucket.h"
#include "block.h"
#include <stdio.h>

Storage create_storage(){
	
	Storage retval;
	retval.numBuckets = INIT_STORAGE_SIZE;
	
	int i;
	for(i=0; i<INIT_STORAGE_SIZE; ++i){
		
		retval.allBuckets[i] = create_dummy_bucket();	
	
	}
	
	
	return retval;

}

void print_storage(Storage toPrint){
	
	int i;
	for(i=0; i<INIT_STORAGE_SIZE; ++i){
		printf("Bucket %d:\n", i);
		print_bucket(toPrint.allBuckets[i]);
	

	}
}
