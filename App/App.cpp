#include <stdio.h>
#include <iostream>

#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
//#include "../Include/data.hpp"
#include "../Include/oramStructs.hpp"
#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
#include "appJoin.hpp"
#include <time.h>
//#include "EncryptionTest.cpp"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;
int inputRate = 10; //rate is in the form of mb/s
bool splitHalf = false;

// OCall implementations
void ocall_println(const char* str) {
    printf("%s\n", str);
    fflush(stdout);
}
void ocall_print(const char* str) {
    printf("%s", str);
    fflush(stdout);
}
int ocall_add(int a, int b){
	printf("Adding outside\n");
	return a + b;
}

void ocall_print_int(size_t a){
	printf("o_p_i: %zu\n", a);
}

void print_storage(Storage justCause){
	int i, j,k;
	for(k = 0; k< INIT_STORAGE_SIZE; k++){
		printf("Bucket %d\n", k);
		for (j = 0; j<MAX_BUCKET_SIZE; j++){
			printf("Block %d\n", j);
			for(i = 0; i<MAX_BLOCK_SIZE; i++){
				printf("%c ", justCause.allBuckets[k].blocks[j].data[i]);
			}
			printf("\n");
		}
	}
}

int contains(int* arr, int len, int val){
	int i;
	for(i=0; i<len; i++){
		if(arr[i] == val){
			return 1;
		}
	}
	return 0;
}

void add(int* arr, int len, int val){
	int i;
	for(i=0; i<len; i++){
		if(arr[i] == 0 || arr[i] == val){
			arr[i] = val;
			return;
		}
	}
	
}



int max(int a, int b){
	if(a>b){
		return a;
	}
	else{
		return b;
	}
}

int min(int a, int b){
	if(a<b){
		return a;
	}
	else{
		return b;
	}
}

int numTuplesMap[11][2];
void add(int time, int idx){
	int i;
	for(i=0; i<11; i++){
		if(numTuplesMap[i][0] == -1 || numTuplesMap[i][0] == time){
			numTuplesMap[i][0] = time;
			numTuplesMap[i][1] = idx;
			return;
		}
	}
}
int get(int time){
	int i;
	for(i=0; i<11; i++){
		if(numTuplesMap[i][0] == time){
			return numTuplesMap[i][1];
		}
	}
}

int endTime = 0; 
uint8_t** genCerealBox(int total, Customer* custs, int numCusts, Order* orders, int numOrder){
	uint8_t** retval = (uint8_t**) malloc(total*sizeof(uint8_t*));
	//serialize the data and place into serialized data array
	// elements are placed in an alternating pattern
	int idx = 0, i, offset;
	int currTime = (int)time(NULL);
	for(i=0; i<numOrder; i++){
		
		if(i<numCusts){
			if(splitHalf && idx > total/2){
				offset = 4;
			}
			else{
				offset = (idx*100)/(inputRate*1024*1024);
			}
			custs[i].expireTime = currTime + offset;
			endTime = max(endTime, currTime + offset);
			add(endTime, idx+1);
			retval[idx] = serialize(&custs[i], NULL, 1);
			idx++;
		}
		if(splitHalf && idx > total/2){
			offset = 4;
		}
		else{
			offset = (idx*100)/(inputRate*1024*1024);
		}
		orders[i].expireTime = currTime + offset;
		endTime = max(endTime, currTime + offset);
		add(endTime, idx+1);
		retval[idx] = serialize(NULL, &orders[i], 0);
		idx++;
	}
	
	endTime +=1;
	return retval;
}

void print_block(Block* toPrint){
	
	int i;
	printf("Data: ");
	for(i=0; i<MAX_BLOCK_SIZE; i++){
		printf("%d ", toPrint->data[i]);
	}
	printf("\nInit Vec: ");
	for(i=0; i<12; i++){
		printf("%d ", toPrint->init_vec[i]);
	}
	printf("\nmac: ");
	for(i=0; i<16; i++){
		printf("%d ", toPrint->mac[i]);
	}
	printf("\n");
}

double print_time(clock_t start, clock_t end, char* message, int print){
	double time_elapsed = double(end - start) / double(CLOCKS_PER_SEC);
	if(print){
		printf("%s %.9f \n", message, time_elapsed);
	}
	return time_elapsed;
}

int main(int argc, char const *argv[]) {

		
	
	/*Block temp;
	Block tempt;
	
	int b;
	for(b=0; b<MAX_BLOCK_SIZE; b++){
		temp.data[b] = b;
	}
	for(b=0; b<12; b++){
		temp.init_vec[b] = b;
	}
	for(b=0; b<16; b++){
		temp.mac[b] = b;
	}
	
	print_block(&temp);
	print_block(&tempt);
	memcpy(&tempt, &temp, sizeof(Block));
		print_block(&temp);
	print_block(&tempt);
	printf("Block Size: %lu -- %d\n", sizeof(Block), 3001+12+16);
	
	abort();
	*/
	//Create the Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }
    
    sgx_status_t status;
    int retval, i, j;
    
    for(i =0; i<11; i++){
    	for(j = 0; j<2; j++){
    		numTuplesMap[i][j] = -1;
    	}
    }

    
    int numCusts = 100;//524288; //100
    int numOrder = 100;//524288; //100
    int total = numCusts+numOrder;
    
    Customer* custs = (Customer*) malloc(numCusts * sizeof(Customer));
    char cpath[] = "dataGen/customerSmall.csv";
	readCust(cpath, numCusts, custs);
	
	Order* orders = (Order*) malloc(numOrder*sizeof(Order));
	char opath[] = "dataGen/orderSmall.csv";
	readOrder(opath, numOrder, orders);	



    int BufferSize = 209*1000;
    
    char outBuffer[BufferSize];
    memset(outBuffer, 0, BufferSize);

	
	uint8_t** cerealBox = genCerealBox(total, custs, numCusts, orders, numOrder);

	
	/*Customer tmpC;
	Order tmpO;
	int tmp[12];
	memset(tmp, 0, 12*sizeof(int));
	for(i=0; i<total; i++){
		
		deserialize(cerealBox[i], &tmpC, &tmpO, isCust(cerealBox[i]));
		
		if(isCust(cerealBox[i])){
			add(tmp, 12, tmpC.expireTime);
			//printf("%s\n", custToStr(tmpC));
		}
		else{
			add(tmp, 12, tmpO.expireTime);
			//printf("%s\n", orderToStr(tmpO));
		}
		
	}
	
	for(i=0; i<12; i++){
		printf("%d\n", tmp[i]);
	}
		
	abort();*/
	clock_t start, end, batchStart, batchEnd;
	int window = 1;
	printf("The window size is %d\n", window);	
	int currTime = (int)time(NULL);
	int batchSize = 5; //number of tuples to process per loop
	int numProcessed = 0;
	int tuplesAvailable = 0;
	char messageB[] = "Time spent on this batch is:";
	int numJoined = 0;
	char message[] = "The time elapsed is";
    double batchProcessTime = 0;
    
    Storage custStore;// = create_storage();
    Storage orderStore;// = create_storage();
    start = clock();
    batchStart = clock();
    status = ecall_sjoin(global_eid, &custStore, &orderStore, cerealBox, 0, window, outBuffer);
    if(status != SGX_SUCCESS){
		return 1;
	}
    batchEnd = clock();
    char messageA[] = "The time spent to just initalize the Storages and Storage Managers";
    print_time(batchStart, batchEnd, messageA, 1);
    
    
    

	
	//run while 
	while(currTime < endTime){
		if(max(tuplesAvailable - numProcessed, 0)){
			batchStart = clock();
			status = ecall_sjoin(global_eid, &custStore, &orderStore, cerealBox+numProcessed, max(min(batchSize, tuplesAvailable-numProcessed+1), 0), window, outBuffer);
			//printf("%d\n", max(min(batchSize, tuplesAvailable-numProcessed+1), 0));

			if(status != SGX_SUCCESS){
				return 1;
			}
			else{
				numProcessed += batchSize;
				//printf("Did %d\n", numProcessed);
			}
			batchEnd = clock();
	
			batchProcessTime += print_time(batchStart, batchEnd, messageB, 0);
		}else{
			tuplesAvailable = get(currTime);	
		
		}
		currTime = (int)time(NULL);
	}
	
	
	
	end = clock();
	
	
	
	
	for(i = 0; i<BufferSize; i++){
		if(outBuffer[i] == ':'){
			numJoined++;
		}
		//printf("%c", outBuffer[i]);
	}
	printf("The Number of joins: %d\n", numJoined);
	
	print_time(start, end, message, 1);
	printf("The time elapsed for all batches is %.9f \n", batchProcessTime);
	//printf("The time for each tuple is %.5f \n", time_elapsed/total); 
	printf("**********************************\n");

	//Now set up the NLJ to get an idea of how many tuples should be joined
	for(i=0; i<total; i++){
		free(cerealBox[i]);
	}
	free(cerealBox);
	cerealBox = genCerealBox(total, custs, numCusts, orders, numOrder);
	Customer* custPlace = (Customer*) malloc(numCusts * sizeof(Customer));
	Order* orderPlace = (Order*) malloc(numOrder*sizeof(Order));
	currTime = (int)time(NULL);
	tuplesAvailable = 0;
	numProcessed = 0;
	batchProcessTime = 0;
	
	start = clock();
		//run while 
	while(currTime < endTime){
		if(max(tuplesAvailable - numProcessed, 0)){
			batchStart = clock();
			//status = ecall_sjoin(global_eid, &custStore, &orderStore, cerealBox+numProcessed, max(min(batchSize, tuplesAvailable-numProcessed+1), 0), window, outBuffer);
			//printf("%d\n", max(min(batchSize, tuplesAvailable-numProcessed+1), 0));
			numJoined = NLJ(custPlace, orderPlace, cerealBox+numProcessed, max(min(batchSize, tuplesAvailable-numProcessed+1), 0), window, 0);
			numProcessed += batchSize;
			//printf("Did %d\n", numProcessed);
			batchEnd = clock();
	
			batchProcessTime += print_time(batchStart, batchEnd, messageB, 0);
		}else{
			tuplesAvailable = get(currTime);	
		
		}
		currTime = (int)time(NULL);
	}
	
	end = clock();
	
	printf("The Number of joins: %d\n", numJoined);
	print_time(start, end, message, 1);
	
	
	//double time_elapsed = double(end - start) / double(CLOCKS_PER_SEC);
	printf("The time elapsed for all batches is %.9f \n", batchProcessTime);
	
	printf("Freeing everything\n");
	free(custs);
	free(orders);
	free(custPlace);
	free(orderPlace);
	for(i=0; i<total; i++){
		free(cerealBox[i]);
	}
	free(cerealBox);
	//printf("Printing Customer\n");
	//print_storage(custStore);
	/*
	printf("Printing Order\n");
	print_storage(orderStore);
	*/
	
	
	//#####################################################
    // Destroy enclave
    status = sgx_destroy_enclave(global_eid);
    if(status != SGX_SUCCESS) {
        std::cout <<  "Fail to destroy enclave.\n" << std::endl;
        return 1;
    }

    return 0;
}
   
