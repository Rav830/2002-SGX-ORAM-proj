#include "Enclave_t.h"
#include "string.h" //<-safe to include as intel made a lib for it
#include "../Include/tableData.h"
//#include "stdlib.h"
#include "Oram/storage.hpp"
#include "Oram/config.h"

int generate_random_number() {
    ocall_print("Processing random number generation...");
    return 42;
}


int enclave_main(int *ptr) {
	ocall_print("==> Entering enclave");
	
	Storage tmp = create_storage();
	int i, j,k;
	for(k = 0; k< INIT_STORAGE_SIZE; k++){
	
		ocall_print("Storage");
		for (j = 0; j<MAX_BUCKET_SIZE; j++){
			ocall_print("Bucket");
			for(i = 0; i<MAX_BLOCK_SIZE; i++){
				ocall_print_int(tmp.allBuckets[k].blocks[j].data[i]);
			}
		}
	}
	ocall_print("<== Exiting enclave");
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
