#include "Enclave_t.h"
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
