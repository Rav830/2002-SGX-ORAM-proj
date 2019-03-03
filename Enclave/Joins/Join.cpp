#include "Enclave_t.h"
#include "../../Include/tableData.h"
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
int ecall_nlj_cartesian(Product** tblA, Customer** tblB, int numProds, int numCusts){
	int i, j;
	//first decrypt the data //unseal(sgx_sealed_data_t* sealed_data, size_t sealed_size, uint8_t* plaintext, uint32_t plaintext_len)
	/*
	char* unsealed;
	for(i = 0; i<numProds; ++i){
		ocall_print("==>raw");
		ocall_print( (*(tblA + i))->id);
		ocall_print(tblA[i]->name);
		ocall_print("<==raw");
		
		unseal( (sgx_sealed_data_t*)tblA[i]->id, sizeof(sgx_sealed_data_t) + sizeof(tblA[i]->id),
				(uint8_t*)&unsealed, sizeof(unsealed) );
		tblA[i]->id = unsealed;
		
		unseal( (sgx_sealed_data_t*)tblA[i]->name, sizeof(sgx_sealed_data_t) + sizeof(tblA[i]->name),
				(uint8_t*)&unsealed, sizeof(unsealed) );
		tblA[i]->name = unsealed;
	
	
		ocall_print("==>decrypt");
		ocall_print(tblA[i]->id);
		ocall_print(tblA[i]->name);
		ocall_print("<==decrypt");
	}
	
	for(i = 0; i<numCusts; ++i){
		ocall_print("==>raw");
		ocall_print(tblA[i]->id);
		ocall_print(tblA[i]->name);
		ocall_print("<==raw");
		unseal( (sgx_sealed_data_t*)tblB[i]->id, sizeof(sgx_sealed_data_t) + sizeof(tblB[i]->id),
				(uint8_t*)&unsealed, sizeof(unsealed) );
		tblB[i]->id = unsealed;
		
		unseal( (sgx_sealed_data_t*)tblB[i]->name, sizeof(sgx_sealed_data_t) + sizeof(tblB[i]->name),
				(uint8_t*)&unsealed, sizeof(unsealed) );
		tblB[i]->name = unsealed;
	
	
		ocall_print("==>decrypt");
		ocall_print(tblB[i]->id);
		ocall_print(tblB[i]->name);
		ocall_print("<==decrypt");
	}
	
	/*
	for(i =0; i<numProds; ++i){
	
		for(j=0; j<numCusts; ++i){
			
			if(strcmp(tblA[i]->id, tblB[i]->id) == 0){
				ocall_print("==>Join");
				ocall_print(tblA[i]->id);
				ocall_print(tblB[i]->id);
				ocall_print(tblA[i]->name);
				ocall_print(tblB[i]->name);
				ocall_print("<==Join");
			
			}
			
		
		
		}
	
	
	}
	*/
	
	
	return 1;

}
