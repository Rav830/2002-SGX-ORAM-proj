#include "config.h"
#include "block.h"
#include "bucket.h"
#include "storage.h"

#include <stdio.h>
#include <string.h>

Block stash[20];
int stashIDX = 0;
int pmBID[3];
int pmIDX[3];

int addBlock(Bucket* barr, Block b){
	int i, j;
	for(i=2; i>-1; i--){
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			if(barr[i].blocks[j].bid == -1){
				barr[i].blocks[j] = b;
				return 1;
			}	
		}
	
	}
	return 0;

}


int getpmID(int bid){
	
	int i, retval = -1;
	for(i = 0; i<3; i++){
		if(pmBID[i] == bid){
			retval = i;
		}
	}
	
	return retval;
}

Block access(Storage* oram, int op, Block* data, int pmID){
	int idx = pmIDX[pmID];
	//generate a new index for the data
	int newidx = rand()%numLeaves(oram->numBuckets);
	
	
	//read in one path and place the contents into the stash Note: we executed 3*MAX_BUCKET_SIZE block reads atm
	Bucket path[3];
	get_buckets(*(oram), idx, path);
	int i,j,k;
	for(i=0; i<3; i++){
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			if(path[i].blocks[j].bid > -1){
				stash[stashIDX] = path[i].blocks[j];
			}
		}
	}
	
	//get_buckets(*(oram), newidx, path);
	/*for(i=1; i<3; i++){
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			if(path[i].blocks[j].bid != -1){
				stash[stashIDX] = path[i].blocks[j];
			}
		}
	}*/
	
	//now the stash contains the Block I want (likely)
	//now is when we decrypt and handle what the operation should do
	//op == 1 is write
	//op == 0 is read
	Block retval;
	if(op == 0){
		for(i = 0; i<stashIDX; i++){
			if(stash[i].bid == pmBID[pmID]){
				
				retval = stash[i];
				data = &(stash[i]);
				
			}
		}
	}
	if(op == 1){
		int saved = 0;
		for(i = 0; i<stashIDX; i++){
			if(stash[i].bid == pmBID[pmID]){
				stash[i] = *(data);
				saved = 1;
				
				break;
			}
		}
		if(saved == 0){
			//if writing and block is not in the stash, then assume it was never in the tree to begin with and add it to the stash
			stash[stashIDX] = *(data);
			stashIDX += 1;
		}
	}	
	//Now that the reading is done we can save the new index into the position map
	pmIDX[pmID] = newidx;
	
	//Now initiate the writeback by creating the path of buckets and then seting them.
	int count = 0, parent, tmpPMIDX, idxParent = idxToParentIndex(idx);
	Bucket newPath[3] = {create_dummy_bucket(), create_dummy_bucket(), create_dummy_bucket()};
	int breakLoop = 0;
	for(i=0; i<stashIDX; i++){
		tmpPMIDX = getpmID(stash[stashIDX].bid);
		//leaf = idxToLeafIndex(tmpPMIDX);
		parent = idxToParentIndex(tmpPMIDX);
		
		//first check if the ith item in the stash fits in the last bucket
		if(tmpPMIDX == idx){
			//if so check to see if we can place it in the last bucket going up
			breakLoop = 0;
			//for(j=2; j>-1; j--){
			for(k=0; k<MAX_BUCKET_SIZE; k++){
				if(newPath[2].blocks[k].bid < 0){
					newPath[2].blocks[k] = stash[i];
					breakLoop = 1;
					break;
				}
			}
			//if(breakLoop){
			//	break;
			//}	
			//}
		}
		
		//if we did not fit the block into the last bucket we should try the next bucket
		//also if the current block can fit in this next block we fill it in.
		if(breakLoop == 0 && parent == idxParent){
			breakLoop = 0;
			for(k=0; k<MAX_BUCKET_SIZE; k++){
				if(newPath[1].blocks[k].bid < 0){
					newPath[1].blocks[k] = stash[i];
					breakLoop = 1;
					break;
				}
			}
			//if(breakLoop){
				//break;
		}
		
		//if we still did not fit the block we will try to place it into the root
		if(breakLoop == 0){
			breakLoop = 0;
			for(k=0; k<MAX_BUCKET_SIZE; k++){
				if(newPath[0].blocks[k].bid < 0){
					newPath[0].blocks[k] = stash[i];
					breakLoop = 1;
					break;
				}
			}
		
		}
		
		//this means we placed the current block into the path
		if(breakLoop == 1){
			//replace with a dummy block so we can clean up the stash later.
			stash[stashIDX] = create_dummy_block();
			count+=1;
			//printf("added to bucket\n");
		}
		
		breakLoop = 0; //reset breakloop
	
	}
	/*for(i=0; i<3; i++){
		printf("\tBucket %d\n", i);
		print_bucket(newPath[i]);
	}
	
	
	printf("Stashidx %d\n count %d\n", stashIDX, count);*/
	//compress the stash if there are blocks inside of it.
	if(stashIDX-count){
		Block stashTemp[stashIDX - count];
	
		//now we clean up the stash so that everything is back in the right spot
		j=0;//using j for a stashTemp index
		for (i=0;i<stashIDX; i++){
			if(stash[i].bid > -1){
				stashTemp[j] = stash[i];
				j+=1;
				//stash[i] = create_dummy_block();
			}
		}
		stashIDX -= count;
		for(i=0; i<stashIDX; i++){
			stash[i] = stashTemp[i];
		}
	}

	//finally we place the buckets into the tree.
	set_buckets(oram, idx, newPath);
	return retval;
	
	/*
	, we do this by creating two bucket arrays that I am going to set_bucket to
	Bucket pathOne[3] = {create_dummy_bucket(), create_dummy_bucket(), create_dummy_bucket()}; //idx
	Bucket pathTwo[3] = {create_dummy_bucket(), create_dummy_bucket(), create_dummy_bucket()}; //newidx
	int tmpPMIDX, blocSuccess;
	for(i = 0; i<stashIDX; i++){
		tmpPMIDX = getpmID(stash[i].bid);
		
		//now determine which bucket array it will go to
		if(tmpPMIDX == idx){
			//add to path one
			blocSuccess = addBlock(pathOne, stash[i]);
			if(blocSuccess == 0){
				printf("Access Failure! Make a bigger ORAM\n\n");
				return;
			}
		}
		if(tmpPMIDX == newidx){
			//add to path two
			blocSuccess = addBlock(pathTwo, stash[i]);
			if(blocSuccess == 0){
				printf("Access Failure! Make a bigger ORAM\n\n");
				return;
			}
		}
		
	}
	
	//now a possibility is that there is stuff in path[1] that needs to be merged with the other path
	//if both leaves share the same parent
	if(idxToParentIndex(idx) == idxToParentIndex(newidx)){
		
	}*/
	
		printf("%d\n", data->bid);
	
	
}



void testcode(){

	char d[MAX_BLOCK_SIZE];
	
	int i;
	for(i=0; i<MAX_BLOCK_SIZE-1; ++i){
		d[i] = i+96-32;
	
	}
	d[MAX_BLOCK_SIZE-1] = '\0';
	
	
	Block b = create_block(5, d);
	
	
	
	printf("%d %s\n", b.bid, b.data);
	
	Block dummy = create_dummy_block();
	
	printf("%d %s\n", dummy.bid, dummy.data);
	
	
	Bucket dumdum = create_dummy_bucket();
	print_bucket(dumdum);

	printf("Testing place block\n");
	int retval = place_block(b, &dumdum);
	printf("place_block status %d\n", retval);
	print_bucket(dumdum);

	Storage test = create_storage();
	
	print_storage(test);
	
	Bucket elems[3];
	
	get_buckets(test, 1, elems);
	
	printf("Trying to print bucket\n");
	
	for(i=0; i<3; i++){
		printf("Block %d\n", i);
		print_bucket(elems[i]);
		elems[i] = create_dummy_bucket();
	}
	
	printf("If I manipulate this will it be reflected in the array");
	
	//elems[0] = create_dummy_bucket();
	
	print_storage(test);
	
	for(i=0; i<3; i++){
		printf("Block %d\n", i);
		print_bucket(elems[i]);
	}
	
	printf("Buffer Line\n");
	
	set_buckets(&test, 1, elems);
	print_storage(test);
	
	printf("___Buffer Line ___\n\n\n");
	printf("Make some Blocks to place into our premade storage\n");
	
	char text[30];
	strcpy(text, "Hello There");
	
	Block one = create_block(1, text);
	Block two = create_block(2, text);
	Block three = create_block(3, text);
	
	//testing addblock
	/*
	addBlock(elems, one);
	for(i=0; i<3; i++){
		printf("\tBlock %d\n", i);
		print_bucket(elems[i]);
	}
	*/
	
	printf("%d %s\n", one.bid, one.data);
	//simulate a new block being placed into the storage
	
	pmBID[0] = 1;
	pmBID[1] = 2;
	pmBID[2] = 3;
		
	access(&test, 1, &one, 0);
	access(&test, 1, &two, 1);
	access(&test, 1, &three, 2);
	
	
	print_storage(test);	

	printf("trying to read a block\n");
	
	Block r;
	
	r = access(&test, 0, &r, 1);
	
	printf("%d %s\n", r.bid, r.data);

}

int main(){

	

	printf("compile\n");
	
	testcode();
	
	
	return 1;
}


