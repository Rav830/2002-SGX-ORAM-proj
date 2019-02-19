/* can not do imports of cstdio or stdio.h because that would be linking an untrusted/standard library into the enclave which is not good, if we want std functionality then we need to ocall it out.
#include <cstdio>
*/
#include "Enclave_t.h"


/*
 *@brief      Performs a simple addition in the enclave
 *
 * @param      a      The first input for our simple addition
 * @param      b      The second input for our simple addition
 *
 * @return     Truthy if addition successful, falsy otherwise.
 */
int ecall_add(int a, int b) {
	ocall_print("Adding numbers ... ");
	
	ocall_print("Doing ocall add to test ocall printing from ecall not");
	int result;
	sgx_status_t succ = ocall_add(&result, a+1, b+1);
	if(succ != SGX_SUCCESS){
		return -1;
	} 
	
	
	ocall_print_int(result);
	
	
    return a + b;
}


/*
 * @brief      This will add an int to each value in side of an array and then assign that each value  
 *
 * @param      arrPtr  The Pointer to the array we want to add to
 * @param      arrLen  Then length of the array
 * @param      addVal  The value to add to each element
 *
 * @return     Truthy if addition successful, falsy otherwise.
 */
int ecall_array_add_no_copy(int *arrPtr, size_t arrLen, int addVal){

	int i;
	for(i =0; i< arrLen; ++i){
		*(arrPtr + i) = *(arrPtr + i) + addVal;
	}

	return 1;

}
