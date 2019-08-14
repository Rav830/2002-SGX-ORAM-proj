#include "Enclave_t.h"
#include "string.h" //<-safe to include as intel made a lib for it
//#include "stdlib.h"
#include "Oram/storage.hpp"
#include "Oram/accessFunc.hpp"
#include "../Include/oramStructs.hpp"
#include "../Include/config.h"
#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
#include "printFunc.hpp"

#include "sgx_tcrypto.h"
#include "sgx_trts.h"

#include "math.h"

#include <stdio.h> /* vsnprintf */

//some time imports and variables
#include <sgx_tae_service.h>
sgx_time_t current_time = NULL;
sgx_time_source_nonce_t time_source_nonce;
int start_time = 0;
bool pse_session_active = false;
//



//Storage Managers kept global
int SMinitialized = 0;
StorageManager custSM;
StorageManager orderSM;

///
int joinCnt = 0;

//this include is for a all of the testing functions that I might use
#include "test.cpp"


Block block_from_serialized(uint8_t* data){
	int id = hash(data);
	//transfer this data to one of the right size
	uint8_t bData[MAX_BLOCK_SIZE];
	memset(bData, 0, MAX_BLOCK_SIZE);
	memcpy(bData, data, TUPLE_SIZE);
	//free(data);
	return create_block(id, bData);
}



void addTuple(Customer* c, Order* o, int isCust, Storage* oram, StorageManager* oramSM){

	uint8_t* cereal = serialize(c, o, isCust);
	Block toEnter = block_from_serialized(cereal);
	int pm_loc = add_bid(oramSM, toEnter.data[0]);
	//printf("(hash, pm, loc) (%d, %d, %d)\n", toEnter.data[0], pm_loc, oramSM->pmIDX[pm_loc]);
	//run a check for a failed add_bid
	if(pm_loc+1){
		//printf("trying access\n");
		access(oram, ORAM_APPEND, &toEnter, pm_loc, oramSM);
	}
	else{
		printf("Add Tuple failed\n");
		print_pm(oramSM);
		abort();
	}
	
	free(cereal);
}

void getTime(sgx_time_t* toSet){
	sgx_status_t status;
	//make sure pse session is running 
	while(!pse_session_active){
		status = sgx_create_pse_session();
		
		if(status == SGX_SUCCESS){
			pse_session_active = true;
		}	
		
	}
	status = sgx_get_trusted_time(toSet, &time_source_nonce);	
	
	while(status != SGX_SUCCESS){
		printf("Got the following error @getTime:\n");
		switch(status){
		
			case SGX_ERROR_INVALID_PARAMETER:
				printf("SGX_ERROR_INVALID_PARAMETER\n");
				break;
			case SGX_ERROR_AE_SESSION_INVALID:
				printf("SGX_ERROR_AE_SESSION_INVALID\n");
				break;
			case SGX_ERROR_SERVICE_UNAVAILABLE:
				printf("SGX_ERROR_SERVICE_UNAVAILABLE\n");
				break;
			case SGX_ERROR_SERVICE_TIMEOUT:
				printf("SGX_ERROR_SERVICE_TIMEOUT\n");
				break;
			case SGX_ERROR_NETWORK_FAILURE:
				printf("SGX_ERROR_NETWORK_FAILURE\n");
				break;
			case SGX_ERROR_OUT_OF_MEMORY:
				printf("SGX_ERROR_OUT_OF_MEMORY\n");
				break;
			case SGX_ERROR_OUT_OF_EPC:
				printf("SGX_ERROR_OUT_OF_EPC\n");
				break;
			case SGX_ERROR_UNEXPECTED:
				printf("SGX_ERROR_UNEXPECTED\n");
				break;
		}
		status = sgx_get_trusted_time(toSet, &time_source_nonce);
	
	}
	
	//sleep for one second
	/*
	sgx_time_t newTime = current_time+1;
	while(current_time < newTime){
		time_retval = sgx_get_trusted_time(&current_time, &time_source_nonce);
	}*/

}

void SymmetricHashJoin(Storage* cust, StorageManager* custSM, Storage* order, StorageManager* orderSM, uint8_t** inputArr, int len, int windowSeconds, char* outputBuffer){

	Order tmpO;
	Customer tmpC;
	

	int i, hashVal, pm, evicted =0, cnt= 0;
	for(i = 0; i<len; i++){
		//printf("Current Time: %d\n", current_time);
		//printf("Time passed: %d \n", current_time-start_time);
		//compute the hash value since we will anyways
		hashVal = hash(inputArr[i]);
		//determine if the current element is a customer or an order
		if(isCust(inputArr[i])){
			//printf("is cust\n");
			deserialize(inputArr[i], &tmpC, NULL, 1);
			
			//printf("\tC:%s\n", custToStr(tmpC));
			pm = look_up_bid(orderSM, hashVal);
			if(pm == -1){
				//this hash val does not exist in the order storage so make a random number
				pm = rand()&HASH_RANGE;
			}
			//use the hash val and get the set of tuples from order that matches
			Block lookIn; 
			access(order, ORAM_READ, &lookIn, pm, orderSM);
			
			//now do a quick check for if the Block retrieved is valid
			if(lookIn.data[0] < HASH_RANGE){
				cnt += 1;
				//This block is valid now loop and do a join if the id's match
				/*int p, Smallcnt = 0;
				for(p = 1; p < MAX_BLOCK_SIZE; p++){
					Smallcnt = Smallcnt + lookIn.data[p];
				}
				printf("Sum of the data in block: %d and %d \n", Smallcnt, lookIn.data[0]);	
				*/
				int j;
				int k;
				evicted = 0;
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+TUPLE_SIZE){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , NULL, &tmpO, 0);
					getTime(&current_time);
					//printf("\t\t%s\n", orderToStr(tmpO));
					//now to handle eviction
					if( tmpO.expireTime < current_time - windowSeconds){
						//we found an element to evict
						//printf("Evicting: %s \n", orderToStr(tmpO));
						evicted++;
						
						//left shift the data by TUPLE_SIZE
						for(k=j; k<MAX_BLOCK_SIZE; k++){
							if(k+TUPLE_SIZE < MAX_BLOCK_SIZE){
								lookIn.data[k] = lookIn.data[k+TUPLE_SIZE];
							}
							else{
								lookIn.data[k] = 0;
							}
						}
						
						//move j back one increment so that we don't skip the new tuple that was placed there
						j = j-TUPLE_SIZE;
					}
					else if(tmpO.id == tmpC.id){
						//we found a match
						joinCnt+=1;
						snprintf((outputBuffer + (joinCnt*209) ), 209, "Match: %s %s\n", custToStr(tmpC), orderToStr(tmpO)); 
						//printf("%d Match: %s %s\n", joinCnt, custToStr(tmpC), orderToStr(tmpO));
						//break; don't break, we might find more tuples that could be matched
					}
				}
				
			}
			
			//printf("Eviction Count: %d \n", evicted);
			if(evicted>0){
				int bid = lookIn.data[0];
				int pmID = look_up_bid(orderSM, bid);
				
				//printf("Opmid %d \n", pmID);
				//printf("Ohash/bid %d \n", lookIn.data[0]); 
				/*int p, cnt = 0;
				for(p = 1; p < MAX_BLOCK_SIZE; p++){
					cnt = cnt + lookIn.data[p];
				}
				printf("Sum of the data in block: %d and %d \n", cnt, lookIn.data[0]);
				*/
				//do a check if the block is empty
				if(lookIn.data[1]){
					//this block has data to place
					access(order, ORAM_WRITE, &lookIn, pmID, orderSM);
				}
				else{
					//if the block is empty we should write back a dummy block which means overwrite the block id/hash value and then write it back 
					lookIn.data[0] = HASH_RANGE;
					access(order, ORAM_WRITE, &lookIn, pmID, orderSM);
					//now remove this particular hash value from the position map
					remove_bid(orderSM, bid);
				}
				evicted = 0;
			}
			
			//now that the join happened or not store this particular tuple into the tree
			addTuple(&tmpC, NULL, 1, cust, custSM);
			
		}
		else{
			//printf("is order\n");
			deserialize(inputArr[i], NULL, &tmpO, 0);
			//printf("\tO:%s\n", orderToStr(tmpO));
			pm = look_up_bid(custSM, hashVal);
			if(pm == -1){
				//this hash val does not exist in the order storage so make a random number
				pm = rand()&HASH_RANGE;
			}
			//use the hash val and get the set of tuples from order that matches
			Block lookIn; 
			access(cust, ORAM_READ, &lookIn, pm, custSM);
			
			//now do a quick check for if the Block retrieved is valid
			if(lookIn.data[0] < HASH_RANGE){
				cnt += 1;
				//This block is valid now loop and do a join if the id's match
				/*int p, Smallcnt = 0;
				for(p = 1; p < MAX_BLOCK_SIZE; p++){
					Smallcnt = Smallcnt + lookIn.data[p];
				}
				printf("Sum of the data in block: %d and %d \n", Smallcnt, lookIn.data[0]);	
				*/
				int j;
				int k;
				evicted = 0;
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+TUPLE_SIZE){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , &tmpC, NULL, 1);
					getTime(&current_time);
					//printf("\t\t%s\n", custToStr(tmpC));
					//printf("\t\t%d\n", tmpC.id);
					if( tmpC.expireTime < current_time - windowSeconds ){
						//we found an element to evict
						//printf("Evicting: %s \n", custToStr(tmpC));
						evicted++;
						
						//left shift the data by TUPLE_SIZE
						for(k=j; k<MAX_BLOCK_SIZE; k++){
							if(k+TUPLE_SIZE < MAX_BLOCK_SIZE){
								lookIn.data[k] = lookIn.data[k+TUPLE_SIZE];
							}
							else{
								lookIn.data[k] = 0;
							}
						}
						
						//move j back one increment so that we don't skip the new tuple that was placed there
						j = j-TUPLE_SIZE;
					}
					else if( tmpO.id == tmpC.id){
						//we found a match
						joinCnt+=1;
						snprintf((outputBuffer + (joinCnt*209) ), 209, "Match: %s %s\n", custToStr(tmpC), orderToStr(tmpO)); 
						//printf("%d Match: %s %s\n", joinCnt, custToStr(tmpC), orderToStr(tmpO));
						//break; don't break, we might find more tuples that could be matched
					}
				}
			}
			//printf("Eviction Count: %d \n", evicted);
			if(evicted>0){
				int bid = lookIn.data[0];
				int pmID = look_up_bid(custSM, lookIn.data[0]);
				//printf("Cpmid %d \n", pmID);
				//printf("Chash/bid %d \n", lookIn.data[0]); 
				/*	int p, cnt = 0;
				for(p = 1; p < MAX_BLOCK_SIZE; p++){
					cnt = cnt + lookIn.data[p];
				}
				printf("Sum of the data in block: %d and %d \n", cnt, lookIn.data[0]);
				*/
				//do a check if the block is empty
				if(lookIn.data[1]){
					//this block has data to place
					access(cust, ORAM_WRITE, &lookIn, pmID, custSM);
				}
				else{
					//if the block is empty we should write back a dummy block which means overwrite the block id/hash value and then write it back 
					lookIn.data[0] = HASH_RANGE;
					access(cust, ORAM_WRITE, &lookIn, pmID, custSM);
					//now remove this particular hash value from the position map
					remove_bid(custSM, bid);
				}
				evicted = 0;
			}
			//now that the join happened or not store this particular tuple into the tree
			addTuple(NULL, &tmpO, 0, order, orderSM);
			
		}	
		
	
	}
	/*
	printf("printing the trees for info\n\n");
	
	printf("*************************\nCustomer\n");
	print_storage_no_dummy(*cust);
	print_stash(custSM, 0);
	
	printf("*************************\nOrder\n");
	print_storage_no_dummy(*order);
	print_stash(orderSM, 0);
	*/
	//printf("The total number of correct Blocks retrived are: %d \nThe total number of joins is: %d \n", cnt, joinCnt);
}

void ecall_sjoin(Storage* cust, Storage* order, uint8_t** inputArr, int len, int windowSeconds, char* outputBuffer){
	//printf("==> Entering enclave\n");

	if(cust->initialized != 1){
		init_storage(cust);
	}
	if(order->initialized !=1){
		init_storage(order);
	}

	if(SMinitialized != 1){
		custSM = create_manager();
		orderSM = create_manager();
		SMinitialized = 1;
	}


	//some printing of the data to see if it is there
	
	/*int i, custBool;
	Customer tmpC;
	Order tmpO;
	for(i=0; i<len; i++){
	
		//printf("%c%c->%d\n", inputArr[i][2], inputArr[i][3],  hash(inputArr[i]));
		/*custBool = isCust(inputArr[i]);
		deserialize(inputArr[i], &tmpC, &tmpO, custBool);
		if(custBool){
			printf("Customer: %s\n", custToStr(tmpC));
			addTuple(&tmpC, &tmpO, custBool, cust, &custSM); 
		}
		else{
			printf("Order: %s\n", orderToStr(tmpO));
			addTuple(&tmpC, &tmpO, custBool, order, &orderSM);
		
		}
	
	}*/
	/*
	print_storage_no_dummy(*order);
	printf("Stash: ===\n");
	print_stash(&orderSM, 0);
	printf("PM: ===\n");
	print_pm(&orderSM);
	printf("******************BUFFER LINE*********************");
	print_storage_no_dummy(*cust);
	printf("Stash: ===\n");
	print_stash(&custSM, 0);
	printf("PM: ===\n");
	print_pm(&custSM);
	*/


	/*for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%c ", cust->allBuckets[0].blocks[0].data[i]);
	
	}
	
	printf("**Buff Line**\n");
	encrypt_block(&(cust->allBuckets[0].blocks[0]), 1);
	
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%c ", cust->allBuckets[0].blocks[0].data[i]);
	
	}
	printf("**Buff Line**\n");
	
	decrypt_block(&(cust->allBuckets[0].blocks[0]));
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%c ", cust->allBuckets[0].blocks[0].data[i]);
	
	}
	printf("**Buff Line**\n");
	*/
	SymmetricHashJoin(cust,&custSM, order, &orderSM, inputArr,len, windowSeconds, outputBuffer);
	

	
	//printf("<== Exiting enclave\n");
}

/*void printTest(){
	char p[10] = { '\0'};
	snprintf(p, 10, "%d\n Blag", 45);
	printf(p);
}*/

int enclave_main() {
	printf("==> Entering enclave\n");
	
	
	/*getTime(&current_time);
	sgx_time_t newTime;
	getTime(&newTime);
	newTime += 5;
	printf("Curr Time: %d\n and target time: %d\n", current_time, newTime);
	while(current_time < newTime){
		getTime(&current_time);
	}
	printf("Slept for 5 seconds\n");
	printf("Curr Time: %d\n and target time: %d\n", current_time, newTime);*/
	//storage_test(ptr);
	//access_func_test(ptr);
	//encryption_test();
	
	//encryptBlock_test();	
	//rand_test();
	//printf("Rand: %d \n", rand());
	//printTest();	
	printf("<== Exiting enclave\n");
	return 0;

}

