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

int idxToParentIndex(int index){
	return (((int)index/2)*3) + 1;
}
int idxToLeafIndex(int index){
	return idxToParentIndex(index) + index%2 + 1;

}

int numLeaves(int numBuckets){
	return (int)((numBuckets-1)*2/3);
}

//This read will logically evict and remove the data from the tree structure.
//When a write back occurs that data will be replaced
void get_buckets(Storage collectFrom, int index, Bucket* retval){
	
	//static Bucket retval[3];
	
	//let's make sure that the index given is correct
	int numEntries = collectFrom.numBuckets - ((int)(collectFrom.numBuckets-1) * (1/3));
	
	if(index >= numEntries){
		printf("I received a index that is greater than the number of entries in this tree\n");
		return -1;
	}
	
	//Collect the elements into the retval
	retval[0] = collectFrom.allBuckets[0];
	int parentIndex = idxToParentIndex(index);
	int leafIndex = idxToLeafIndex(index);
	retval[1] = collectFrom.allBuckets[parentIndex];
	retval[2] = collectFrom.allBuckets[leafIndex];
	
	//logicially evict the blocks of data from the tree
	/*int i,j;
	for(i =0; i<3; i++){
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			collectFrom.allBuckets[i].blocks[j].bid = -1;
		}	
	}*/
	
	
	//return retval; 
	
}


//set the buckets in the pointer buckets to the right value in the path
//Note Bucket should be ordered from top of path to leaf
void set_buckets(Storage* toPlace, int index, Bucket* buckets){
	
	int parentIndex = idxToParentIndex(index);
	int leafIndex = idxToLeafIndex(index);
	
	
	toPlace->allBuckets[0] = buckets[0];
	toPlace->allBuckets[parentIndex] = buckets[1];
	toPlace->allBuckets[leafIndex] = buckets[2];
	

}

/* returns truthy for success or failure
success: 0, the block was placed successfully in that path
failure: 1, the block was not able to be placed in that path (either find another location or leave in stash

toplace is the storage that will store this block of data at the path specified by index
*/

int place_block_in_storage(Storage* toPlace, Block data, int index){
	//let's make sure that the index given is correct
	int numEntries = toPlace->numBuckets - ((int)(toPlace->numBuckets-1) * (1/3));
	int retval = 0;
	
	if(index >= numEntries){
		printf("I received a index that is greater than the number of entries in this tree\n");
		return -1;
	}

	//make a path array from leaf to root	
	int path[3] = {idxToLeafIndex(index), idxToParentIndex(index), 0}; 
	
	
	//With the indexes now set, we check to make sure that each bucket in the storacge is either full empty
	int i;
	int j;
	//for each bucket in the storage
	for(i =0; i<3; i++){
		//for each block in that bucket
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			//Check if this is a valid location to place the block
			if( toPlace->allBuckets[ path[i] ].blocks[j].bid == -1){
				toPlace->allBuckets[ path[i] ].blocks[j] = data;
				retval = 1;
				break;
			}
		
		}
		//We placed the block
		if(retval){
			break;
		}
		
	}
	
	return retval;

}






