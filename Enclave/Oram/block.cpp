#include "block.hpp"
//#include "../../Include/oramStructs.hpp"
//#include "config.h"
#include "Enclave_t.h"
#include "../printFunc.hpp"

#include "sgx_tcrypto.h"
#include "sgx_trts.h"



int cnt = 0;
uint8_t p_key[16] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

Block create_block(uint8_t bid, uint8_t data[]){


	Block retval;
	
	//retval.bid = bid;
	
	
	retval.data[0] = bid;
	int i;
	for(i=0; i <MAX_BLOCK_SIZE; ++i){
		retval.data[i+1] = data[i];
	
	}
	
	return retval;

}

void encrypt_block(Block* toEncrypt){
	//make a new init vec for every encrypt
	sgx_read_rand((unsigned char*)&(toEncrypt->init_vec), 12);
	//memset(toEncrypt->mac, 0, 16);
	uint8_t newData[MAX_BLOCK_SIZE];
	//uint8_t p_out_mac[16];
	sgx_status_t retval = sgx_rijndael128GCM_encrypt((sgx_aes_gcm_128bit_key_t*)p_key, toEncrypt->data, MAX_BLOCK_SIZE, newData, toEncrypt->init_vec, 12, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)toEncrypt->mac);
	if(retval != 0){
		printf("\n\nSomething happened with Encryption: %d\n\n", retval);
		abort();
	}
//	int i;
//		for(i=0; i<MAX_BLOCK_SIZE; i++){
	//	printf("%d ",	newData[i]);
	
	//}
	//printf("\n\n\n");
	memcpy(toEncrypt->data, newData, MAX_BLOCK_SIZE);
}
void decrypt_block(Block* toDecrpyt){
	uint8_t newData[MAX_BLOCK_SIZE];
	sgx_status_t retval = sgx_rijndael128GCM_decrypt((sgx_aes_gcm_128bit_key_t*)p_key, toDecrpyt->data, MAX_BLOCK_SIZE, newData, toDecrpyt->init_vec, 12, NULL, 0,(sgx_aes_gcm_128bit_tag_t*)toDecrpyt->mac);
	if(retval != 0){
		printf("\n\nSomething happened with Decryption: %d\n\n", retval);
		printf("Printing block trying to decrypy\n");
		
		int i;
		for(i=0; i<MAX_BLOCK_SIZE; i++){
			printf("%d ", toDecrpyt->data[i]);
		}
		printf("\n{");
		for(i=0; i<12; i++){
			printf("%d, ", toDecrpyt->init_vec[i]);
		}
		printf("}\n{");
		for(i=0; i<16; i++){
			printf("%d, ", toDecrpyt->mac[i]);
		}
		printf("}\n");
		abort();
	}
	memcpy(toDecrpyt->data, newData, MAX_BLOCK_SIZE);
}



Block create_dummy_block(int doEncrypt){
	Block retval;
	memset(retval.data, 0, MAX_BLOCK_SIZE);
	retval.data[0] = HASH_RANGE;
	//int initHelperVars = 1;
	if(doEncrypt){
		encrypt_block(&retval);
	}
	/*
	//retval.bid = -1;
	retval.data[0] = (rand()%(255-HASH_RANGE) )+ HASH_RANGE ;
	int i;
	for (i=1; i< MAX_BLOCK_SIZE; ++i){
		retval.data[i] =(uint8_t) ' ' + ((int)cnt%(126-33));
		cnt+=1;
	}
	//retval.data[MAX_BLOCK_SIZE-1] = '\0';
	*/
	return retval;	
	

}


void print_block(Block toPrint){
	int j;
	printf("%d ", toPrint.data[0]);
	for(j=1; j<MAX_BLOCK_SIZE; j++){
		printf("%c", toPrint.data[j]);
	}
}

