#include "Enclave_t.h"


//Let's make this store large enough
int store[50];
size_t maxSize = 50;
int save = -1;

void ecall_store_int_arr(int* input, size_t len){
	if (len < maxSize){
		int i;
		for(i =0; i<len; ++i){
			store[i] = *(input + i);
			ocall_print_int(*(input+i));
		}
	
	}

}

void ecall_store_int(int input){
	
	save = input;
	
	ocall_print_int(save);
}

void ecall_mult_int(int multBy){
	int i; 
	for(i = 0; i<maxSize; ++i){
		store[i] = store[i]*multBy;
		ocall_print_int(store[i]);
	}
}

//What we do is load the data into an external pointer to allocate it.
void ecall_load_int_arr(int* outPtr, size_t numElems){
	int i;
	for (i=0; i<numElems; ++i){
		*(outPtr + i) = store[i];
	}
	
	
}

//below is helper code from ayeks.de

/*
#include "enclave_test_save_t.h"

#include "sgx_trts.h"
#include <cstring>

char savedString[100] = "Default Enclave savedText";
int savedInt = -1;

// change a buffer with a constant string
void enclaveChangeBuffer(char *buf, size_t len)
{
	const char *secret = "Hello Enclave!";
	if (len > strlen(secret))
	{
		memcpy(buf, secret, strlen(secret) + 1);
	} else {
		memcpy(buf, "false", strlen("false") + 1);
	}
}

// write a var to the buffer
void enclaveStringSave(char *input, size_t len) {
	if ((strlen(input) + 1) < 100)
	{
		memcpy(savedString, input, strlen(input) + 1);		
	} else {
		memcpy(input, "false", strlen("false") + 1);
	}
}

// save the buffer to a var
void enclaveStringLoad(char *output, size_t len) {
	if (len > strlen(savedString))
	{
		memcpy(output, savedString, strlen(savedString) + 1);
	} else {
		memcpy(output, "false", strlen("false") + 1);
	}
}

// save a int to a var
void enclaveSaveInt(int input) {
	savedInt = input;
}

// return a var
int enclaveLoadInt() {
	return savedInt;
}*/
