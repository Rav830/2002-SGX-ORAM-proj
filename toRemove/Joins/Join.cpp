///////////////////////
// Currently Unused
//////////////

#include "Enclave_t.h"
#include "../../Include/data.hpp"
#include "string.h"
#include "sgx_trts.h"
#include "sgx_tseal.h"


void ecall_read_product(Product* a){
	ocall_print("hit read");
	ocall_print_int(a->id);
	ocall_print(a->name);

}

//assume join id is id
//using in to allocate locally
int ecall_nlj(Product** prodTbl, Customer** custTbl, int numProds, int numCusts){
	ocall_print("\n\n\njoin content\nid, Customer, Product");
	//Assume Customer is smaller
	//and just print the contents 
	int i, j;
	/*
	for(j=0; j<numCusts; ++j){
		ocall_print("==>Print Line");
		ocall_print_int(custTbl[j]->id);
		ocall_print(custTbl[j]->name);
	}*/
	
	for(i = 0; i< numCusts; ++i){
		for(j=0; j<numProds; ++j){
			//check if the id's match
			if(custTbl[i]->id == prodTbl[j]->id){
				ocall_print("==>Join Line");
				ocall_print_int(custTbl[i]->id);
				ocall_print(custTbl[i]->name);
				ocall_print(prodTbl[j]->name);
				ocall_print("<==Join Line");
			}
		}
	} 


	return 1;
}
	
