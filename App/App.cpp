#include <stdio.h>
#include <iostream>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"
//#include "../Include/vars.cpp"

/* Global EID shared by multiple threads */
sgx_enclave_id_t global_eid = 0;

int pull[20] = {1,2,3,4,5,6,7,8,9,0,9,8,7,6,5,4,3,2,1,0};

int num;

// OCall implementations
void ocall_print(const char* str) {
    printf("%s\n", str);
}

int ocall_add(int a, int b){
	printf("Adding outside\n");
	return a + b;
}

void ocall_print_int(int a){
	printf("The enclave said to print the following int: %d\n", a);
}

/*int[] ocall_get_arr(){
	return pull; 
}*/


int main(int argc, char const *argv[]) {
	//Create the Enclave
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        std::cout << "Fail to initialize enclave." << std::endl;
        return 1;
    }
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
	
	//#####################################################
    // Seal the random number
    size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(ptr);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    sgx_status_t ecall_status;
    status = seal(global_eid, &ecall_status,
            (uint8_t*)&ptr, sizeof(ptr),
            (sgx_sealed_data_t*)sealed_data, sealed_size);

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
        return 1;
    }

    int unsealed;
    status = unseal(global_eid, &ecall_status,
            (sgx_sealed_data_t*)sealed_data, sealed_size,
            (uint8_t*)&unsealed, sizeof(unsealed));

    if (!is_ecall_successful(status, "Unsealing failed :(", ecall_status)) {
        return 1;
    }

    std::cout << "Seal round trip success! Receive back " << unsealed << std::endl;
	//#####################################################
	
	printf("\n\nSome Buffer Space\n\n");
	
	//#####################################################
	//Let's try to copy some array's into the enclave and 
	// then do some generic operations on them through ocalls
/*
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
*/	
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

	//#####################################################
    // Destroy enclave
    status = sgx_destroy_enclave(global_eid);
    if(status != SGX_SUCCESS) {
        std::cout <<  "Fail to destroy enclave.\n" << std::endl;
        return 1;
    }

    return 0;
}
