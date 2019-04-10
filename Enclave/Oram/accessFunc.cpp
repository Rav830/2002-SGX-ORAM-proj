#include "storage.hpp"
#include "config.h"

#include <math.h>


int getpmID(StorageManager* oramSM, int bid){
	
	int i, retval = -1;
	for(i = 0; i<3; i++){
		if(oramSM->pmBID[i] == bid){
			retval = i;
		}
	}
	
	return retval;
}

//return the largest int where a tree would match indicies
//@return a valid index for the bucket array to set
int intersect_idx(int leafOne, int leafTwo, int height, int numBuckets, int leaves){
	int leafOneIdx = numBuckets - leaves + leafOne;
	int leafTwoIdx = numBuckets - leaves + leafTwo;
	int i = height - 1;
	while (leafOneIdx > -1 && leafTwoIdx > -1 && i > -1){
		if(leafOneIdx == leafTwoIdx){
			return i;
		}
		
		leafOneIdx = ((leafOneIdx+1)>>1)-1;
		leafTwoIdx = ((leafTwoIdx+1)>>1)-1;
		i--;
	}
	return i;
	//return height - abs( (leafOne>>1) - (leafTwo>>1) ) - 1;
	
}


Block access(Storage* oram, int op, Block* data, int pmID, StorageManager* oramSM){
	int idx = oramSM->pmIDX[pmID];
	//generate a new index for the data
	int newidx = rand()%oram->leaves;
	//printf("new location: %d\n", newidx);
	
	
	//read in one path and place the contents into the stash Note: we executed TREE_HEIGHT*MAX_BUCKET_SIZE block reads atm
	Bucket path[oram->height];
	get_buckets(oram, idx, path);
	int i,j,k;
	
	//for each bucket
	for(i=0; i<oram->height; i++){
		//for each block in bucket
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			//store in stash if it is valid
			if(path[i].blocks[j].data[0] < HASH_RANGE){
				//what do I do if the element is already in the stash???
				oramSM->stash[oramSM->stashIDX] = path[i].blocks[j];
				oramSM->stashIDX++;
			}
		}
	}
	

	Block retval = create_dummy_block();
	int saved = 0;
	//read
	if(op == ORAM_READ){
		for(i = 0; i<oramSM->stashIDX; i++){
			if(oramSM->stash[i].data[0] == oramSM->pmBID[pmID]){
				
				retval = oramSM->stash[i];
				data = &(oramSM->stash[i]);
					
			}
		}
	}
	
	
	//data modification
	if(op == ORAM_WRITE || op == ORAM_APPEND){
		//write (i.e. overwrite with new data)
			//make sure that data is the proper array
		if(op == ORAM_WRITE){
			for(i = 0; i<oramSM->stashIDX; i++){
				if(oramSM->stash[i].data[0] == oramSM->pmBID[pmID]){
					//we found the data, let overwrite it.
					memset(oramSM->stash[i].data, 0, MAX_BLOCK_SIZE);
					for(j=0; j<MAX_BLOCK_SIZE; j++){
						oramSM->stash[i].data[j] = data->data[j];
					}
					saved = 1;
				
					break;
				}
			}
		}
		//append
		//on append we write the first 64 valid elements into the block on the array.
		if(op==ORAM_APPEND){
			for(i = 0; i<oramSM->stashIDX; i++){
				//we found the block we want to manipulate
				
				//data is 65
				//block is 641 ish
				//data needs to be added to increments of 64 starting from 1 to 641 - 64 
				if(oramSM->stash[i].data[0] == oramSM->pmBID[pmID]){
					//printf("hit data found\n");
					saved = 1;
					int p, k;
					//search for an open spot
					for(p=1; p <= MAX_BLOCK_SIZE - 64; p = p + 64){
						if(oramSM->stash[i].data[p] == 0){
							//found the index let's shove the data in
							break;
						}
					}
					//printf("The index found is: %d\n", p);
					//error check for whether we can't fit an element into the block
					if(p > MAX_BLOCK_SIZE - 64){
						ocall_print("\n\nWe exceeded the number of elements in this block, Please increase the size of the block or change the hashing strategy\n\n");
						abort();
					
					}
				
					for(k=1; k < 65; k++){
						oramSM->stash[i].data[p] = data->data[k];
						p++;
					}
				
					break;
				}
			}
		}
		//if writing and block is not in the stash, then assume it was never in the tree to begin with and add it to the stash
		//this applies to either append or write
		if(saved == 0){
			//printf("making a new save\n");
			memset(oramSM->stash[oramSM->stashIDX].data, 0, MAX_BLOCK_SIZE);
			for(j=0; j<65; j++){
				oramSM->stash[oramSM->stashIDX].data[j] = data->data[j];
			}
			oramSM->stashIDX += 1;
		}
	
		
	}	
	//Now that the reading is done we can save the new index into the position map
	oramSM->pmIDX[pmID] = newidx;
	
	//Now initiate the writeback by creating the path of buckets and then seting them.

	Bucket newPath[oram->height];
	for(i=0; i<oram->height; i++){
		newPath[i] = create_dummy_bucket();
	}
	
	int breakLoop = 0, count = 0, leafIDX, numMatch;
	//for each element in the stash
	for(i=0; i<oramSM->stashIDX; i++){
		//printf("Stash item %d %s\n",  
		//get leaf index
		leafIDX = oramSM->pmIDX[getpmID(oramSM,oramSM->stash[i].data[0])];
		//now we need to determine which bucket it should go in
		numMatch = intersect_idx(leafIDX, idx, oram->height, oram->numBuckets, oram->leaves);
		for(j = numMatch; j > -1; j--){
			for(k=0; k < MAX_BUCKET_SIZE; k++){
				//look for a dummy block to store the values into
				if(newPath[j].blocks[k].data[0] >= HASH_RANGE){
					newPath[j].blocks[k] = oramSM->stash[i];
					
					//mark this in the stash that it is dummy
					oramSM->stash[i].data[0] = (rand()%(255-HASH_RANGE) )+ HASH_RANGE ;
					count++;
					breakLoop = 1;
					break;
				}
			}
			if(breakLoop){
				breakLoop = 0;
				break;
			}
		
		}
		
		
	}

	
	//compress the stash if there are blocks inside of it.
	//Algorithm generally follows a move everything into a new stash and then move everything back.
	if(oramSM->stashIDX-count){
		Block stashTemp[oramSM->stashIDX - count];
	
		//now we clean up the stash so that everything is back in the right spot
		j=0;//using j for a stashTemp index
		for (i=0;i<oramSM->stashIDX; i++){
			if(oramSM->stash[i].data[0] < HASH_RANGE){
				stashTemp[j] = oramSM->stash[i];
				j+=1;
			}
		}
		oramSM->stashIDX -= count;
		for(i=0; i<oramSM->stashIDX; i++){
			oramSM->stash[i] = stashTemp[i];
		}
	}else{
		oramSM->stashIDX = 0;
	}


	//finally we place the buckets into the tree.
	set_buckets(oram, idx, newPath);
	return retval;
	
}
