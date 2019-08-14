#include "storage.hpp"
#include "bucket.hpp"
#include "block.hpp"
#include <math.h>
#include "Enclave_t.h"
#include "../printFunc.hpp"
//#include <stdio.h>
//#include <stdint.h>

int counter[256];

Storage create_storage(){
	
	Storage retval;
	retval.leaves = INIT_STORAGE_ELEMS;
	retval.height = log(retval.leaves)/log(2)+1;
	retval.numBuckets = (int)(pow(2, retval.height)-1);
	retval.initialized = 1;
	
	int i;
	for(i=0; i<retval.numBuckets; ++i){
		
		retval.allBuckets[i] = create_dummy_bucket(1);	
	
	}
	
	
	return retval;

}
void init_storage(Storage* retval){
	retval->leaves = INIT_STORAGE_ELEMS;
	retval->height = log(retval->leaves)/log(2)+1;
	retval->numBuckets = (int)(pow(2, retval->height)-1);
	retval->initialized = 1;
	int i;
	for(i=0; i<retval->numBuckets; ++i){
		
		retval->allBuckets[i] = create_dummy_bucket(1);	
	
	}

}

void print_storage(Storage toPrint){
	
	int i;
	for(i=0; i<toPrint.numBuckets; ++i){
		printf("Bucket %d:\n", i);
		print_bucket(toPrint.allBuckets[i]);
	}
}

void print_storage_no_dummy(Storage toPrint){
	
	int i;
	for(i=0; i<toPrint.numBuckets; ++i){
		printf("Bucket %d:\n", i);
		print_bucket_no_dummy(toPrint.allBuckets[i]);
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
void get_buckets(Storage* collectFrom, int index, Bucket* retval){


	
	//static Bucket retval[3];
	
	//let's make sure that the index given is correct
	
	if(index >= collectFrom->leaves || index <= -1){
		ocall_print("I received a index that is greater than the number of entries in this tree\n");
		abort();
	}
	

	//Collect the elements into the retval
	int i = collectFrom->height-1;
	int treeIdx = collectFrom->numBuckets - collectFrom->leaves + index;
	while (treeIdx > -1 && i > -1){
		
		retval[i] = collectFrom->allBuckets[treeIdx];
		int j;
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			decrypt_block( &(retval[i].blocks[j]) );
		}
		
		//go up the tree to the parent
		treeIdx = ((treeIdx+1)>>1)-1;
		i--;
	}
	
	/*

	printf("===================\nPrinting Blocks collected from tree\n");
	//for each bucket
	int j, k;
		for(k=0; k<256; k++){
		counter[k] = 0;
	}
	for(i = 0; i<=collectFrom->height-1; i++){
		//for each block
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			//reset a counter to summarize the blocks
			//memset(counter, 0 , 256);
			for(k =0; k<MAX_BLOCK_SIZE; k++){
				counter[retval[i].blocks[j].data[k]]++;
			}
			
			for(k=0; k<256; k++){
				if(counter[k]){
					printf("%d:%d\t", k, counter[k]);
				}
				counter[k] = 0;
			}
			printf("\n===\n===\n");
			
		}
	
	}
	
	
	/*retval[0] = collectFrom.allBuckets[0];
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


	/*
	printf("===================\nPrinting Blocks before encryption\n");
	//for each bucket
	
			for(k=0; k<256; k++){
		counter[k] = 0;
	}

	for(i = 0; i<=toPlace->height-1; i++){
		//for each block
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			//reset a counter to summarize the blocks
			//memset(counter, 0 , 256);
			for(k =0; k<MAX_BLOCK_SIZE; k++){
				counter[buckets[i].blocks[j].data[k]]++;
			}
			
			for(k=0; k<256; k++){
				if(counter[k]){
					printf("%d:%d\t", k, counter[k]);
				}
				counter[k] = 0;
			}
			//memset(counter, 0 , 256);
			printf("\n===\n===\n");
			
		}
	
	}
	
	*/
	//let's pre encrypt all the data before copying it back out

	int i, j, k;
	for(j = 0; j<=toPlace->height-1; j++){
		for(k = 0; k<MAX_BUCKET_SIZE; k++){
			encrypt_block( &(buckets[j].blocks[k]) );
		}
	}
	/*
		printf("===================\nPrinting Blocks after encryption\n");
	//for each bucket

	for(i = 0; i<=toPlace->height-1; i++){
		//for each block
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			//reset a counter to summarize the blocks
			//memset(counter, 0 , 256);
			for(k =0; k<MAX_BLOCK_SIZE; k++){
				counter[buckets[i].blocks[j].data[k]]++;
			}
			
			for(k=0; k<256; k++){
				if(counter[k]){
					printf("%d:%d\t", k, counter[k]);
				}
				counter[k] = 0;
			}
			//memset(counter, 0 , 256);
			printf("\n===\n===\n");
			
		}
	
	}
	*/
	
	i = toPlace->height-1;
	int treeIdx = toPlace->numBuckets - toPlace->leaves + index;
	while (treeIdx > -1 && i > -1){
		//printf("\t\t\t Writing to: %d \t %d\n", i, treeIdx);
		toPlace->allBuckets[treeIdx] = buckets[i];
		
		/*
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			//reset a counter to summarize the blocks
			//memset(counter, 0 , 256);
			for(k =0; k<MAX_BLOCK_SIZE; k++){
				counter[ buckets[i].blocks[j].data[k]]++;
			}
			
			for(k=0; k<256; k++){
				if(counter[k]){
					printf("%d:%d\t", k, counter[k]);
				}
				counter[k] = 0;
			}
			//memset(counter, 0 , 256);
			printf("\n===\n===\n");
			
	
	}
		*/
		//go up the tree to the parent
		treeIdx = ((treeIdx+1)>>1)-1;
		i--;
	}
	
	/*printf("===================\nPrinting Blocks Sent to tree\n");
	treeIdx = toPlace->numBuckets - toPlace->leaves + index;
	while (treeIdx > -1){
		printf("\t\t%d\n", treeIdx); 
		toPlace->allBuckets[treeIdx];
		
				

		//for each block
		for(j = 0; j<MAX_BUCKET_SIZE; j++){
			//reset a counter to summarize the blocks
			//memset(counter, 0 , 256);
			for(k =0; k<MAX_BLOCK_SIZE; k++){
				counter[ toPlace->allBuckets[treeIdx].blocks[j].data[k]]++;
			}
			
			for(k=0; k<256; k++){
				if(counter[k]){
					printf("%d:%d\t", k, counter[k]);
				}
				counter[k] = 0;
			}
			//memset(counter, 0 , 256);
			printf("\n===\n===\n");
			
	
	}
		//go up the tree to the parent
		treeIdx = ((treeIdx+1)>>1)-1;
	}
	*/
	
	/*int parentIndex = idxToParentIndex(index);
	int leafIndex = idxToLeafIndex(index);
	
	
	toPlace->allBuckets[0] = buckets[0];
	toPlace->allBuckets[parentIndex] = buckets[1];
	toPlace->allBuckets[leafIndex] = buckets[2];
	*/

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
		ocall_print("I received a index that is greater than the number of entries in this tree\n");
		abort();
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
			if( toPlace->allBuckets[ path[i] ].blocks[j].data[0] >= HASH_RANGE){
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


//all this really does is set the pmbid to be all -1 for later, otherwise it sets things to 0 just to be helpful
//initialize the pm locations to 
StorageManager create_manager(){

	StorageManager retval;
	retval.stashIDX = 0;
	int i;
	for(i=0; i < PM_SIZE; i++){
		retval.pmBID[i] = -1;
		retval.pmIDX[i] = 0;//rand()%HASH_RANGE;
	}
	
	
	
	return retval;

}


int add_bid(StorageManager* lookIn, int bid){
	int i;
	//Doing two loops one to try to find the element first and another to actually add it to the first available spot
	//printf("adding bid %d \n", bid);
	//print_pm(lookIn);
	for(i=0; i<PM_SIZE; i++){
		if(lookIn->pmBID[i] == bid){
			return i;
		}
	}
	
	for(i=0; i<PM_SIZE; i++){
		if(lookIn->pmBID[i] == -1){
			lookIn->pmBID[i] = bid;	
			return i;
		}
	}
	
	
	ocall_print("Ran out of space in the position map\n\n");
	
	return -1;

}

int remove_bid(StorageManager* lookIn, int bid){
	int i;
	//printf("Removing: %d\n", bid);
	for(i=0; i<PM_SIZE; i++){
		//printf("\t\t\t%d\n", lookIn->pmBID[i]);	
		if(lookIn->pmBID[i] == bid){
			//printf("\n");
			lookIn->pmBID[i] = -1;
			return 1;
		}
	}
	
	printf("failed to find the block id (%d) in this ORAM Stroage Manager\n\n", bid);
	abort();
	return -1;

}

int look_up_bid(StorageManager* lookIn, int bid){
	int i;
	for(i=0; i<PM_SIZE; i++){
		//printf("\t\t\t%d\n", lookIn->pmBID[i]);
		if(lookIn->pmBID[i] == bid){
			return i;
		}
	}
	return -1;		
}

void print_stash(StorageManager* lookIn, int printDummy){
	
	int i;
	for(i=0; i< lookIn->stashIDX; i++){
		if(lookIn->stash[i].data[0] < HASH_RANGE || printDummy){
			printf("Block %d\n\t", i);
			print_block(lookIn->stash[i]);
			printf("\n");
			
		
		}
	
	}

}

void print_pm(StorageManager* toPrint){
	printf("(BID, IDX)\n");
	int i;
	for(i =0; i<PM_SIZE; i++){
		printf("(%d, %d)\n", toPrint->pmBID[i], toPrint->pmIDX[i]);
	
	}

}


