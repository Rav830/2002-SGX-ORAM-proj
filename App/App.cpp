#include <stdio.h>
#include <iostream>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
//#include "../Include/data.hpp"
#include "../Include/oramStructs.hpp"
#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
//#include "Join/appJoin.cpp"


//#include "EncryptionTest.cpp"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

int pull[20] = {0};

int num;

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

/*int[] ocall_get_arr(){
	return pull; 
}

void testJoin(){
	//first prepare the simulated input stream
	Customer custs[20];
	readCust("../dataGen/customer.csv", 20, custs);
	
	Order orders[80];
	readOrder("../dataGen/order.csv", 80, orders);	
	
	uint8_t* cerealBox[100];
	
	
	int idx = 0, i;
	
	//serialize the data and place into serialized data array
	for(i=0; i<80; i++){
		
		if(i<20){
			cerealBox[idx] = serialize(&custs[i], NULL, 1);
			idx++;
		}
		
		cerealBox[idx] = serialize(NULL, &orders[i], 0);
		idx++;
	}
	// elements are placed in an alternating pattern

	printf("The data has been read in and serialized into an array, creating two storages and going to join the data\n");	

	StorageManager custSM = create_manager();
	
	Storage orderStore = create_storage();
	StorageManager orderSM = create_manager();
	
	SymmetricHashJoin(&custStore, &custSM, &orderStore, &orderSM, cerealBox, 100);

}
*/


int main(int argc, char const *argv[]) {

	//Create the Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }
    
    sgx_status_t status;
    
    Storage justCause;
    //print_storage(justCause);
    int retval;
    //printf("SGX SUCCESS: %d\n", SGX_SUCCESS);
    //printf("SGX_ERROR_INVALID_PARAMETER: %d\n", SGX_ERROR_INVALID_PARAMETER);
    //printf("SGX_ERROR_OUT_OF_MEMORY: %d\n", SGX_ERROR_OUT_OF_MEMORY);
    //printf("SGX_ERROR_UNEXPECTED: %d\n", SGX_ERROR_UNEXPECTED ); 
    
    
    Customer custs[20];
    char cpath[] = "dataGen/customer.csv";
	readCust(cpath, 20, custs);
	
	Order orders[80];
	char opath[] = "dataGen/order.csv";
	readOrder(opath, 80, orders);	
	
	uint8_t* cerealBox[100];
	
	
	int idx = 0, i;
	
	//serialize the data and place into serialized data array
	for(i=0; i<80; i++){
		
		if(i<20){
			cerealBox[idx] = serialize(&custs[i], NULL, 1);
			idx++;
		}
		
		cerealBox[idx] = serialize(NULL, &orders[i], 0);
		idx++;
	}
	// elements are placed in an alternating pattern

    Storage custStore;// = create_storage();
    Storage orderStore;// = create_storage();
    
    int BufferSize = 209*1000;
    
    char outBuffer[BufferSize];
    memset(outBuffer, 0, BufferSize);
    	for(i = 0; i<BufferSize; i++){
		printf("%d ", outBuffer[i]);
	}
    
    status = ecall_sjoin(global_eid, &custStore, &orderStore, cerealBox, 50, outBuffer);
    if(status != SGX_SUCCESS){
		return 1;
	}
	//printf("Printing Customer\n");
	//print_storage(custStore);
	
	//status = ecall_sjoin(global_eid, &custStore, &orderStore, (cerealBox+1), 1, outBuffer);
	if(status != SGX_SUCCESS){
		return 1;
	}
	//printf("Printing Customer\n");
	//print_storage(custStore);
	//status = ecall_sjoin(global_eid, &custStore, &orderStore, (cerealBox+2), 1, outBuffer);
	if(status != SGX_SUCCESS){
		return 1;
	}
	
	for(i = 0; i<BufferSize; i++){
		printf("%c", outBuffer[i]);
	}
	//printf("Printing Customer\n");
	//print_storage(custStore);
	/*printf("**********************************\n");
	printf("Printing Order\n");
	print_storage(orderStore);
	*/
	
    
    //#####################################################
    // Seal a string
    //int ptr = 100;
    /*char ptr[100] = "abcdef";
    //memset(ptr, 0, 100);
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(char)*100;
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);
    int i;
	char unsealed[100];

    sgx_status_t ecall_status;
    status = seal(global_eid, &ecall_status,
            (uint8_t*)ptr, 100*sizeof(char), //plain text and plaintext len???
            (sgx_sealed_data_t*)sealed_data, sealed_size); //sealed data and sealed ??

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
        return 1;
    }
    
    
    printf("sealed_size = %lu\n", sealed_size);
    for(i=0; i<sealed_size; i++){
    	printf("%d ", sealed_data[i]);
    }
	printf("\n");
   
    status = unseal(global_eid, &ecall_status,
            (sgx_sealed_data_t*)sealed_data, sealed_size,
            (uint8_t*)unsealed, 100*sizeof(char));

    if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
        return 1;
    }

    std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;

	printf("BUFFERLINE*********************************************\n");
    status = seal(global_eid, &ecall_status,
            (uint8_t*)ptr, 100*sizeof(char), //plain text and plaintext len???
            (sgx_sealed_data_t*)sealed_data, sealed_size); //sealed data and sealed ??

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
        return 1;
    }
    
    
    printf("sealed_size = %lu\n", sealed_size);
    for(i=0; i<sealed_size; i++){
    	printf("%d ", sealed_data[i]);
    }
	printf("\n");
   
    status = unseal(global_eid, &ecall_status,
            (sgx_sealed_data_t*)sealed_data, sealed_size,
            (uint8_t*)unsealed, 100*sizeof(char));

    if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
        return 1;
    }

    std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;
    */
    
	//#####################################################
    
    
     /*
    int id = 1;
    char name[] = "prodName";
    
    Product* p = createProduct(id, name, 0);
    
    setGlobalID(global_eid);
    
    printProd(p);
    
    ecall_read_product(global_eid, p); 
    
    /*
    Block* root = (Block*)malloc(sizeof(Block));
    root->id = 5;
    root->content = "root";
    
    Block* left = (Block*)malloc(sizeof(Block));
    left->id = 6;
    left->content = "adaf";
    
    Block* right = (Block*)malloc(sizeof(Block));
    right->id = 7;
    right->content = "adfa";
    
    root->left = left;
    root->right = right;
    
    Block* leftleft = (Block*)malloc(sizeof(Block));
    leftleft->id = 8;
    leftleft->content = "leftleft";
    
    root->left->left = leftleft;
    
    printf("%d %s\n", root->id, root->content);
    printf("%d %s\n", root->left->id, root->left->content);
    
    //ecall_print_block(global_eid, root);
    */
    

    /*int i;    
    Product** p_arr = genProducts(50);
    Customer** c_arr = genCustomers(5);
    
    
    for(i =0; i< 20; ++i){
    	printProd(p_arr[i]);
    }
 	printf("==========\n");
    for(i =0; i < 5 ; ++i){
    	printCust(c_arr[i]);
    }
    
    NLJ(p_arr, 50, c_arr, 5);
    
    
    int retval;
    
    status = ecall_nlj(global_eid, &retval, p_arr, c_arr, 50, 5);
    if(status != SGX_SUCCESS) {
        std::cout <<  "Failed to join.\n" << std::endl;
        return 1;
    }
    
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
    /*
    
   
    
    //#####################################################
    //Random number generation 
    int ptr;
    sgx_status_t status = generate_random_number(global_eid, &ptr);
    std::cout << status << std::endl;
    if (status != SGX_SUCCESS) {
        std::cout << "noob" << std::endl;
    }
    printf("Random number: %d\n", ptr);
	//#####################################################
	   //#####################################################
    // Seal a string
    //int ptr = 100;
    char ptr[100]; //"abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi1abcdefghi";
    memset(ptr, 0, 100);
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(char)*100;
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);
    int i;
	char unsealed[100];

    sgx_status_t ecall_status;
    status = seal(global_eid, &ecall_status,
            (uint8_t*)ptr, 100*sizeof(char), //plain text and plaintext len???
            (sgx_sealed_data_t*)sealed_data, sealed_size); //sealed data and sealed ??

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
        return 1;
    }
    
    
    printf("sealed_size = %lu\n", sealed_size);
    for(i=0; i<sealed_size; i++){
    	printf("%d ", sealed_data[i]);
    }
	printf("\n");
   
    status = unseal(global_eid, &ecall_status,
            (sgx_sealed_data_t*)sealed_data, sealed_size,
            (uint8_t*)unsealed, 100*sizeof(char));

    if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
        return 1;
    }

    std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;

	//#####################################################

	//#####################################################
    // Perform a simple addition in the enclave
    int a = ptr;
    int b = 100;
    status = ecall_add(global_eid, &ptr, a, b);
    if (status != SGX_SUCCESS) {
        std::cout << "Enclave addition failed :(" << std::endl;
        return 1;
    }
    std::cout << "Enclave addition success! " << a << " + " << b << " = " << ptr << std::endl;
    //#####################################################

	//#####################################################
	//Perform simple multiplication in the enclave
	
	int x = 34;
	int y = 2;
	int retval;
	status = ecall_multiply(global_eid, &retval, x, y);
	
	if (status != SGX_SUCCESS) {
		std::cout << "Enclave multiplication failed :(" << std::endl;
        return 1;
	}	
	
	
	
	std::cout << "Enclave multiplication success! " << x << " * " << y << " = " << retval << std::endl;
    //#####################################################
	
	
	
	printf("\n\nSome Buffer Space\n\n");
	
	//#####################################################
	//Let's try to copy some array's into the enclave and 
	// then do some generic operations on them through ocalls

	//make int array
	
	int data[20];
	size_t size = 20;
	int i;
	for(i = 0; i < size; ++i){
		data[i] = i;
	}
	
	//now we do some ecall to store this information into the enclave and let's just ocall print it
	
		//Let's store an int
	status = ecall_store_int(global_eid, data[5]);
	
	if(status != SGX_SUCCESS){
		return 1;
	}
	printf("\nSome Buffer Space\n");
	status = ecall_store_int_arr(global_eid, data, size);
	if(status != SGX_SUCCESS){
		return 1;
	}
	
	printf("\nSome Buffer Space\n");
	status = ecall_mult_int(global_eid, 7);
	if(status != SGX_SUCCESS){
		return 1;
	}
	printf("\nSome Buffer Space\n");
	//make something to hold data
	int outHold[20];
	
	status = ecall_load_int_arr(global_eid, outHold, size);
	if(status != SGX_SUCCESS){
		return 1;
	}
	
	for(i=0; i<size; ++i){
		printf("outHold has %d\n", outHold[i]);
	
	}
	
	//#####################################################
    // interact with external var
	
	int* numPTR = &num;

	*numPTR = 5;
		
	printf("%d\n", *numPTR);
	printf("%d\n", num);
	status = enclave_main(global_eid, &retval, &num);
	if(status != SGX_SUCCESS){
		return 1;
	}
	printf("%d\n", *numPTR);
    
    num = 65;
    printf("%d\n", num);
    status = enclave_main(global_eid, &retval, &num);
	if(status != SGX_SUCCESS){
		return 1;
	}
    
    printf("%d\n", *numPTR);
    
    
    
    //make an int array
    int data[20];
	size_t size = 20;
	int i;
	for(i = 0; i < size; ++i){
		data[i] = i;
		printf("%d\n", data[i]);
	}
	
	
	status = ecall_array_add_no_copy(global_eid, &retval, data, size, 6);
    if(status != SGX_SUCCESS){
		return 1;
	}
    
    
	for(i=0; i<size; ++i){
		printf("%d\n", data[i]);
	
	}
	
    //#####################################################

	status = enclave_fill_mem(global_eid);
    if(status != SGX_SUCCESS){
		return 1;
	}*/
