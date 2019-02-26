#include "Enclave_t.h"
#include "string.h" //<-safe to include as intel made a lib for it
//#include "../Include/vars.cpp"

int generate_random_number() {
    ocall_print("Processing random number generation...");
    return 42;
}


int enclave_main(int *ptr) {
	ocall_print("==> Entering enclave");
	
	ocall_print_int(*ptr);
	
	ocall_print("Umm lemme try to manipulate a variable that is not in the enclave.");
	
	*ptr = 9;
	
	ocall_print_int(*ptr);

	ocall_print("<== Exiting enclave");
	return 0;

}

void enclave_fill_mem(){
	int *m =(int*) calloc(5, sizeof(int));
	*(m + 1) = 3;



}
