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

int generate_random_number() {
    ocall_println("Processing random number generation...");
    return 42;
}

void print_storage(Storage* outside){
	int i, j,k;
	for(k = 0; k< INIT_STORAGE_SIZE; k++){
		printf("Bucket %d\n", k);
		for (j = 0; j<MAX_BUCKET_SIZE; j++){
			printf("Block %d\n", j);
			for(i = 0; i<MAX_BLOCK_SIZE; i++){
				printf("%d ", outside->allBuckets[k].blocks[j].data[i]);
			}
			printf("\n");
		}
	}

}

void storage_test(Storage* outside){
	//Storage tmp = create_storage();
	init_storage(outside);
	
}
Block block_from_serialized(uint8_t* data){
	int id = hash(data);
	//transfer this data to one of the right size
	uint8_t bData[MAX_BLOCK_SIZE];
	memset(bData, 0, MAX_BLOCK_SIZE);
	memcpy(bData, data, TUPLE_SIZE);
	return create_block(id, bData);
}


void access_func_test(Storage* outside){
	init_storage(outside);
	//printf("%d\n", rand());
	StorageManager outsideSM = create_manager();
	Block toEnter;
	memset(toEnter.data, 5, MAX_BLOCK_SIZE);
	int pm_loc = add_bid(&outsideSM, toEnter.data[0]);
	printf("Location: %d\n", pm_loc);
	printf("outsideSM idx: %d\n", outsideSM.stashIDX);	 
	//printf("(hash, pm, loc) (%d, %d, %d)\n", toEnter.data[0], pm_loc, oramSM->pmIDX[pm_loc]);
	//run a check for a failed add_bid
	if(pm_loc+1){
		printf("trying access\n");
		access(outside, ORAM_WRITE, &toEnter, pm_loc, &outsideSM);
	}
	else{
		printf("Add Tuple failed\n");

	}
	
	print_storage(outside);
	
		

}


void encryption_test(){

	//set the key for encryption
	uint8_t p_key[16];
	memset(p_key, 1, 16);
	//p_key[0] = 1;
	
	//set the data to be encrpyted
	char text_src[] = "Text To Encrypt";
	//char text_src[15];
	//memset(text_src, 4, 15);
	uint32_t text_len = strlen(text_src);
	
	
	//initialization vector
	uint32_t iv_len = 12; 
	uint8_t iv[iv_len];
	memset(iv, 3, iv_len);
	
	uint32_t aad_len = 0;
	const uint8_t* aad = NULL;
	
	//output
	char* text_encrypt = (char*)malloc(text_len*sizeof(char));
	uint8_t p_out_mac[16];
	memset(p_out_mac, 0, 16);
	int i;
	
	printf("Before Encryption details\n");
	
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	
	printf("Encrypting...\n");
	
	sgx_status_t retval = sgx_rijndael128GCM_encrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_src, text_len, (uint8_t*)text_encrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval==SGX_ERROR_INVALID_PARAMETER);
	
	printf("After Encryption Details\n");
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	
	printf("Decrypting....\n");
	char text_decrypt[text_len];
	retval = sgx_rijndael128GCM_decrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_encrypt, text_len, (uint8_t*)text_decrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval);
	printf("\t Dencrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", text_decrypt[i]);
	}
	printf("\n");

	
	printf("Encrypting...\n");
	memset(iv, 4, iv_len);
	retval = sgx_rijndael128GCM_encrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_src, text_len, (uint8_t*)text_encrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval);
	
	printf("After Encryption Details\n");
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	printf("Decrypting....\n");
	char text_decrypt_two[text_len];
	memset(p_key, 5, 16);
	retval = sgx_rijndael128GCM_decrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_encrypt, text_len, (uint8_t*)text_decrypt_two, iv, iv_len, NULL, 0, NULL);
	
	printf("SGX_Status: %d\n", retval== SGX_ERROR_MAC_MISMATCH);
	printf("\t Dencrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", text_decrypt_two[i]);
	}
	printf("\n");
	
	
	

}

void rand_test(){
	printf("Creating an array of 10 random numbers\n");
	uint8_t buffer[12];
	memset(buffer, 0, 12);
	sgx_read_rand(buffer, 12);
	int i;
	for(i=0; i<12; i++){
		printf("%d ", buffer[i]);
	}
	printf("\n");


	printf("Creating a single random number (32 bit signed) (doing it 10 times)\n");
	int num = 0;
	for(i=0; i<10; i++){
		sgx_read_rand((unsigned char*) &num, 4);
		printf("%d\n", num);
	}
}

void addTuple(Customer* c, Order* o, int isCust, Storage* oram, StorageManager* oramSM){

	uint8_t* cereal = serialize(c, o, isCust);
	Block toEnter = block_from_serialized(cereal);
	int pm_loc = add_bid(oramSM, toEnter.data[0]);
	printf("(hash, pm, loc) (%d, %d, %d)\n", toEnter.data[0], pm_loc, oramSM->pmIDX[pm_loc]);
	//run a check for a failed add_bid
	if(pm_loc+1){
		printf("trying access\n");
		access(oram, ORAM_APPEND, &toEnter, pm_loc, oramSM);
	}
	else{
		printf("Add Tuple failed\n");
		print_pm(oramSM);
		abort();
	}
	
	free(cereal);
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
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+TUPLE_SIZE){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , NULL, &tmpO, 0);
					if( tmpO.id == tmpC.id){
						//we found a match
						printf("Match: %s <-> %s\n", custToStr(tmpC), orderToStr(tmpO));
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
				for(j = 1; j<MAX_BLOCK_SIZE && lookIn.data[j]; j = j+TUPLE_SIZE){
					//deserialize the data located here into tmp
					deserialize( (lookIn.data+j) , &tmpC, NULL, 1);
					if( tmpO.id == tmpC.id){
						//we found a match
						printf("Match: %s <-> %s\n", custToStr(tmpC), orderToStr(tmpO));
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

void ecall_sjoin(Storage* cust, Storage* order, uint8_t** inputArr, int len){
	printf("==> Entering enclave\n");
	init_storage(cust);
	init_storage(order);
	StorageManager custSM = create_manager();
	StorageManager orderSM = create_manager();



	//some printing of the data to see if it is there
	/*
	int i, custBool;
	Customer tmpC;
	Order tmpO;
	for(i=0; i<len; i++){
	
		custBool = isCust(inputArr[i]);
		deserialize(inputArr[i], &tmpC, &tmpO, custBool);
		if(custBool){
			printf("Customer: %s\n", custToStr(tmpC));
			addTuple(&tmpC, &tmpO, custBool, cust, &custSM); 
		}
		else{
			printf("Order: %s\n", orderToStr(tmpO));
			addTuple(&tmpC, &tmpO, custBool, order, &orderSM);
		
		}
	
	}
	
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
	SymmetricHashJoin(cust,&custSM, order, &orderSM, inputArr,len);
	

	
	printf("<== Exiting enclave\n");
}

/*void printTest(){
	char p[10] = { '\0'};
	snprintf(p, 10, "%d\n Blag", 45);
	printf(p);
}*/

int enclave_main(Storage* ptr) {
	printf("==> Entering enclave\n");
	
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

void enclave_fill_mem(){
	ocall_print("Setting Memory\n");
	uint8_t fillMem[128*5000];
	ocall_print("Filling Memory\n");
	int i;
	//for(i=0; i<128*2000; i++){
	//	fillMem[i] = (uint8_t)120;
	//}
	memset(fillMem, 133, 128*5000);



}

void ecall_testStruct(testB* obj){
	printf("==> Entering enclave\n");
	printf("c: %d\n", obj->c);
	
	testA data;
	data.a = 1;
	data.b = 5;
	
	obj->d = data;
	
	printf("a: %d\n", obj->d.a);
	printf("b: %d\n", obj->d.b);
	
	printf("B Addr: %p\n", (void*)obj);
	printf("A Addr: %p\n", (void*)&(obj->d));
	printf("data Addr: %p\n", (void*)&data);
	
	printf("<== Exiting enclave\n");
}

/*



/*
void ecall_print_block(Block* root){
	ocall_print("==> in enclave");
	ocall_print("Root info");
	ocall_print_int(root->id);
	ocall_print(root->content);
	ocall_print("Left info");
	ocall_print_int(root->left->id);
	ocall_print(root->left->content);
	ocall_print("Right info");
	ocall_print_int(root->right->id);
	ocall_print(root->right->content);
	ocall_print("leftleft info");
	ocall_print_int(root->left->left->id);
	ocall_print(root->left->left->content);	
	ocall_print("<== Exiting enclave");
}*/
