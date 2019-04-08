#include "config.h"
#include "block.h"
#include "bucket.h"
#include "storage.h"
#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <math.h>

//Block stash[20];
//int stashIDX = 0;
//int pmBID[5];
//int pmIDX[5];

int addBlock(Bucket* barr, Block b){
	int i, j;
	for(i=2; i>-1; i--){
		for(j=0; j<MAX_BUCKET_SIZE; j++){
			if(barr[i].blocks[j].data[0] >= HASH_RANGE){
				barr[i].blocks[j] = b;
				return 1;
			}	
		}
	
	}
	return 0;

}


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
	
	//printf("**Printing path read\n");
	//for(i=0; i< oram->height; i++){
	//	printf("i: %d\n", i);
	//	print_bucket_no_dummy(path[i]);
	//}
	
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
	//op == 2 is append
	//op == 1 is write
	//op == 0 is read
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
				//printf("Stashes Hash Val: %d\n", oramSM->stash[i].data[0]);
				//printf("\tInput Hash Val: %d\n", oramSM->pmBID[pmID]);
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
						printf("\n\nWe exceeded the number of elements in this block, Please increase the size of the block or change the hashing strategy\n\n");
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
	//printf("count - %d\n", count);
	//for each element in the stash
	for(i=0; i<oramSM->stashIDX; i++){
		//printf("Stash item %d %s\n",  
		//get leaf index
		leafIDX = oramSM->pmIDX[getpmID(oramSM,oramSM->stash[i].data[0])];
		//now we need to determine which bucket it should go in
		numMatch = intersect_idx(leafIDX, idx, oram->height, oram->numBuckets, oram->leaves);
		//printf("This bucket should be: %d\n", numMatch);
		for(j = numMatch; j > -1; j--){
			for(k=0; k < MAX_BUCKET_SIZE; k++){
				//look for a dummy block to store the values into
				if(newPath[j].blocks[k].data[0] >= HASH_RANGE){
					//printf("found dummy\n");
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
	
	/*
		//leaf = idxToLeafIndex(tmpPMIDX);
		//parent = idxToParentIndex(tmpPMIDX);
		
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
		*/
	/*for(i=0; i<3; i++){
		printf("\tBucket %d\n", i);
		print_bucket(newPath[i]);
	}
	
	==*/
	//printf("Stashidx %d\n count %d\n", oramSM->stashIDX, count);
	//printf("stash contents\n");
	//for(i=0; i<STASH_SIZE; i++){
	//	printf("\t%d\n", oramSM->stash[i].data[0]);
	//}

	//printf("**Printing path to be written\n");
	//for(i=0; i< oram->height; i++){
	//	printf("i: %d\n", i);
	//	print_bucket_no_dummy(newPath[i]);
	//}
	
	
	
	//printf("***Printing Stash\n");	
	//print_stash(oramSM, 1);
	
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
				//stash[i] = create_dummy_block();
			}
		}
		oramSM->stashIDX -= count;
		for(i=0; i<oramSM->stashIDX; i++){
			oramSM->stash[i] = stashTemp[i];
		}
	}else{
		oramSM->stashIDX = 0;
	}
	//printf("***Printing Stash\n");	
	//print_stash(oramSM, 1);

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
	
	//printf("%d\n", data->bid);
	
	
}



void testcode(){

	char d[MAX_BLOCK_SIZE];
	
	int i;
	for(i=0; i<MAX_BLOCK_SIZE-1; ++i){
		d[i] = i+96-32;
	
	}
	d[MAX_BLOCK_SIZE-1] = '\0';
	
	
	Block b = create_block(5, (uint8_t*)d);
	
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%d ", b.data[i]);
	}
	printf("\n");
	
	//printf("%d %s\n", b.data[0], b.data);
	
	Block dummy = create_dummy_block();
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%d ", dummy.data[i]);
	}
	printf("\n");
	
	
	Bucket dumdum = create_dummy_bucket();
	print_bucket(dumdum);
	
	printf("Testing place block\n");
	int retval = place_block(b, &dumdum);
	printf("place_block status %d\n", retval);
	print_bucket(dumdum);

	Storage test = create_storage();
	StorageManager testSM;
	
	print_storage(test);
	
	Bucket elems[test.height];
	
	get_buckets(&test, 1, elems);
	
	printf("\nTrying to print bucket\n");
	
	for(i=0; i<test.height; i++){
		printf("Bucket %d\n", i);
		print_bucket(elems[i]);
		elems[i] = create_dummy_bucket();
	}
	
	printf("\nIf I manipulate this will it be reflected in the array\n\n");
	
	elems[0] = create_dummy_bucket();
	printf("Printing storage\n");
	print_storage(test);
	
	printf("\nPrinting new bucket\n");
	for(i=0; i<test.height; i++){
		printf("Block %d\n", i);
		print_bucket(elems[i]);
	}
	
	printf("\nSetting bucket and printing\n");
	
	set_buckets(&test, 1, elems);
	print_storage(test);
	
	printf("___Buffer Line ___\n\n\n");
	printf("Make some Blocks to place into our premade storage\n");
	
	char text[MAX_BLOCK_SIZE];
	memset(text, 0, MAX_BLOCK_SIZE);
	strcpy(text, "Hello There");
	
	Block one = create_block(1, (uint8_t*)text);
	Block two = create_block(2, (uint8_t*)text);
	Block three = create_block(3, (uint8_t*)text);
	
	//testing addblock
	
	//addBlock(elems, one);
	//for(i=0; i<3; i++){
	//	printf("\tBlock %d\n", i);
	//	print_bucket(elems[i]);
	//}
	
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%d ", one.data[i]);
	}
	printf("\n");
	//printf("%d %s\n", one.bid, one.data);
	//simulate a new block being placed into the storage
	
	testSM.pmBID[0] = 1;
	testSM.pmBID[1] = 2;
	testSM.pmBID[2] = 3;
		
	access(&test, ORAM_WRITE, &one, 0, &testSM);
	access(&test, ORAM_WRITE, &two, 1, &testSM);
	access(&test, ORAM_WRITE, &three, 2, &testSM);
	
	
	print_storage(test);

	printf("trying to read a block\n");
	
	Block r;
	
	r = access(&test, ORAM_READ, &r, 1, &testSM);
	printf("success\n");
	//printf("%d %s\n", r.bid, r.data);
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%d ", r.data[i]);
	}
	printf("\n");
	
	printf("\n did the location of block 2 change\n");
	//print_storage(test);	
	

}

void testFileIO(){
	Customer custs[10];
	readCust("../dataGen/customer.csv", 10, custs);
	Order orders[200];
	readOrder("../dataGen/order.csv", 200, orders);
	
	int i;
	for(i=0; i<10; i++){
		printf("%s\n", custToStr(custs[i])); 
	}
	

	

	for(i=0; i<20; i++){
		printf("%s\n", orderToStr(orders[i])); 
	}
	
	printf("serializing\n");

	uint8_t* serializedCust = serialize(&custs[0], NULL, 1);
	for(i=0; i<64; i++){
		printf("%c", serializedCust[i]);
	}

	printf("\n");
	
	uint8_t* serializedOrd = serialize(NULL, &orders[0], 0);
	for(i=0; i<64; i++){
		printf("%d", serializedOrd[i]);
	}
	printf("\n");
	printf("deserializing\n");
	
	Customer out;
	deserialize(serializedCust, &out, NULL, 1);
	printf("%s\n", custToStr(out));

	Order outo;
	deserialize(serializedOrd, NULL, &outo, 0);
	printf("%s\n", orderToStr(outo));
	
	//printf("Let's try hashing the serialized data");

}


Block block_from_serialized(uint8_t* data){
	int id = hash(data);
	//transfer this data to one of the right size
	uint8_t bData[MAX_BLOCK_SIZE];
	memset(bData, 0, MAX_BLOCK_SIZE);
	memcpy(bData, data, 64);
	//int i;
	//for(i = 0; i<64; i++){
	//	bData[i] = data[i];
	//}

	return create_block(id, bData);

}

void addTuple(Customer* c, Order* o, int isCust, Storage* oram, StorageManager* oramSM){

	uint8_t* cereal = serialize(c, o, isCust);
	Block toEnter = block_from_serialized(cereal);
	int pm_loc = add_bid(oramSM, toEnter.data[0]);
	//printf("(hash, pm, loc) (%d, %d, %d)\n", toEnter.data[0], pm_loc, oramSM->pmIDX[pm_loc]);
	//run a check for a failed add_bid
	if(pm_loc+1){
		access(oram, ORAM_APPEND, &toEnter, pm_loc, oramSM);
	}
	else{
		printf("Add Tuple failed\n");
		abort();
	}
	
	free(cereal);
}


void readAndStore(){
	printf("reading custs\n");
	Customer custs[10];
	readCust("../dataGen/customer.csv", 10, custs);
	
	Order orders[20];
	readOrder("../dataGen/order.csv", 20, orders);
	
	//quick serialize with hash check
	/*uint8_t* sereal = serialize(&custs[0], NULL, 1);
	uint8_t* sereal2 = serializeWithHash(&custs[0], NULL, 1);
	
	for(i=0; i<64; i++){
		printf("%d ", sereal[i]);
	}
	printf("\n");
	
	for(i=0; i<65; i++){
		printf("%d ", sereal2[i]);
	}
	printf("\n");
	
	Customer dS;
	deserialize(sereal, &dS, NULL, 1);
	
	printf("%s\n", custToStr(dS));
	
	Customer dSH;
	deserializeWithHash(sereal2, &dSH, NULL, 1);
	printf("%s\n", custToStr(dSH));
	
	abort();*/
	
	printf("Printing content with hash to check that it works right\n");
	int i;
	char* tmp;
	uint8_t* cereal;
	int hashVal;
	for(i=0; i<10; i++){
		tmp = custToStr(custs[i]);
		printf("%s\n", tmp); 
		cereal = serialize(&custs[i], NULL, 1);
		hashVal = hash(cereal);
		printf("\t%d\n", hashVal);
		
		free(tmp);
		free(cereal);
	}

	for(i=0; i<20; i++){
		tmp = orderToStr(orders[i]);
		printf("%s\n", tmp);
		cereal = serialize(NULL, &orders[i], 0);
		hashVal = hash(cereal);
		printf("\t%d\n", hashVal);
		free(tmp);
		free(cereal); 
	}
	printf("\n");
	printf("Now we make 2 storages and managers and store this information.\n");
	
	Storage custStore = create_storage();
	StorageManager custSM = create_manager();
	
	Storage orderStore = create_storage();
	StorageManager orderSM = create_manager();
	
	
	for(i=0; i<10; i++){
		addTuple(&custs[i], NULL, 1, &custStore, &custSM);
		//printf("%d\n", i);
		//print_storage_no_dummy(custStore);
		//printf("Stash IDX: %d\n", custSM.stashIDX);
		//printf("*************BUFFER LINE\n\n");
	}
	
	for(i=0; i<20; i++){
		addTuple(NULL,&orders[i], 0, &orderStore, &orderSM);
		//printf("%d\n", i);
		//print_storage_no_dummy(custStore);
		//printf("Stash IDX: %d\n", custSM.stashIDX);
		//printf("*******Printing Storage************\n");
		//print_storage_no_dummy(orderStore);
		//printf("*************BUFFER LINE\n\n");
	}
	
	
	print_storage_no_dummy(custStore);
	if(custSM.stashIDX){
		printf("There is stuff in the stash contents\n");
		print_stash(&custSM, 0);
		printf("\n");
	}
	else{
		printf("There is nothing in this stash\n");
	}
	printf("*************BUFFER LINE\n\n");
	print_storage_no_dummy(orderStore);
	if(orderSM.stashIDX){
		printf("There is stuff in the stash contents\n");
		print_stash(&orderSM, 0);
		printf("\n");
	}
	else{
		printf("There is nothing in this stash\n");
	}
		
	//for(i=0; i<PM_SIZE; i++){
	//	printf("pmVals: %d\n", orderSM.pmBID[i]);
	//}
	
}

void SymmetricHashJoin(Storage* cust, StorageManager* custSM, Storage* order, StorageManager* orderSM, uint8_t** inputArr, int len){

	Order tmpO;
	Customer tmpC;
	int i, hashVal, pm;
	for(i = 0; i<len; i++){
		//compute the hash value since we will anyways
		hashVal = hash(inputArr[i]);
		//determine if the current element is a customer or an order
		if(isCust(inputArr[i])){
			printf("is cust\n");
			deserialize(inputArr[i], &tmpC, NULL, 1);
			
			pm = look_up_bid(orderSM, hashVal);
			if(pm == -1){
				//this hash val does not exist in the order storage so make a random number
				pm = rand()&HASH_RANGE;
			}
			//use the hash val and get the set of tuples from order that matches
			Block lookIn; 
			lookIn = access(order, ORAM_READ, &lookIn, pm, orderSM);
			
			//now do a quick check for if the Block retrieved is valid
			if(lookIn.data[0] < HASH_RANGE){
				//This block is valid now loop and do a join if the id's match
				
				int j;
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+64){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , NULL, &tmpO, 0);
					if( tmpO.id == tmpC.id){
						//we found a match
						printf("Match: %s %s\n", custToStr(tmpC), orderToStr(tmpO));
						break;
					}
				}
			}
			
			//now that the join happened or not store this particular tuple into the tree
			addTuple(&tmpC, NULL, 1, cust, custSM);
			
		}
		else{
			printf("is order\n");
			deserialize(inputArr[i], NULL, &tmpO, 0);
			
			pm = look_up_bid(custSM, hashVal);
			if(pm == -1){
				//this hash val does not exist in the order storage so make a random number
				pm = rand()&HASH_RANGE;
			}
			//use the hash val and get the set of tuples from order that matches
			Block lookIn; 
			lookIn = access(cust, ORAM_READ, &lookIn, pm, custSM);
			
			//now do a quick check for if the Block retrieved is valid
			if(lookIn.data[0] < HASH_RANGE){
				//This block is valid now loop and do a join if the id's match
				
				int j;
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+64){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , &tmpC, NULL, 1);
					if( tmpO.id == tmpC.id){
						//we found a match
						printf("Match: %s %s\n", custToStr(tmpC), orderToStr(tmpO));
						break;
					}
				}
			}
			
			//now that the join happened or not store this particular tuple into the tree
			addTuple(NULL, &tmpO, 0, order, orderSM);
			
		}	
		
	
	}
	
	printf("printing the trees for info\n\n");
	
	printf("*************************\nCustomer\n");
	print_storage_no_dummy(*cust);
	print_stash(custSM, 0);
	
	printf("*************************\nOrder\n");
	print_storage_no_dummy(*order);
	print_stash(orderSM, 0);

} 

void testJoin(){
	//first prepare the simulated input stream
	Customer custs[10];
	readCust("../dataGen/customer.csv", 10, custs);
	
	Order orders[20];
	readOrder("../dataGen/order.csv", 20, orders);	
	
	uint8_t* cerealBox[30];
	
	
	int idx = 0, i;
	
	//serialize the data and place into serialized data array
	for(i=0; i<20; i++){
		
		if(i<10){
			cerealBox[idx] = serialize(&custs[i], NULL, 1);
			idx++;
		}
		
		cerealBox[idx] = serialize(NULL, &orders[i], 0);
		idx++;
	}
	// elements are placed in an alternating pattern

	printf("The data has been read in and serialized into an array, creating two storages and going to join the data\n");	
	Storage custStore = create_storage();
	StorageManager custSM = create_manager();
	
	Storage orderStore = create_storage();
	StorageManager orderSM = create_manager();
	
	SymmetricHashJoin(&custStore, &custSM, &orderStore, &orderSM, cerealBox, 30);

}
void test_insert_idx(){
	printf("Correct / Found\n");
	printf("3 / %d\n", intersect_idx(0, 0, 4, 15, 8));
	printf("2 / %d\n", intersect_idx(0, 1, 4, 15, 8));
	printf("1 / %d\n", intersect_idx(0, 2, 4, 15, 8));
	printf("1 / %d\n", intersect_idx(0, 3, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(0, 4, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(0, 5, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(0, 6, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(0, 7, 4, 15, 8));
	printf("3 / %d\n", intersect_idx(1, 1, 4, 15, 8));
	printf("1 / %d\n", intersect_idx(1, 2, 4, 15, 8));
	printf("1 / %d\n", intersect_idx(1, 3, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(1, 4, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(1, 5, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(1, 6, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(1, 7, 4, 15, 8));
	printf("1 / %d\n", intersect_idx(7, 5, 4, 15, 8));
	printf("0 / %d\n", intersect_idx(5, 3, 4, 15, 8));
}

int main(){

	

	printf("compile\n");
	
	
	//test_insert_idx();
	
	//printf("%d", (int)(pow(2, log(INIT_STORAGE_ELEMS)/log(2)+1)-1));
	
	//testcode();
	
	//readAndStore();
	testJoin();
	//testFileIO();
	return 1;
}


