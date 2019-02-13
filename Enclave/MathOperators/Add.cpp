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
