#include "../Include/tableData.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

sgx_enclave_id_t global_id = 0;
int N_IDS[5] = {0,1,2,3,4};
char NAMES[5][10] = {"Townie","Simonette" , "Dinnie", "Merell", "Madelon"};
int name_idx = -1;
char PRODS[10][20] = {"Jolt Cola", "Breadfruit", "Cream Soda", "Corn", "Gatorade", "Lettuce",  "Bib", "Bread", "Dinner", "Chicken"};


void setGlobalID(sgx_enclave_id_t input){
	global_id = input;	
}

/* Seal the random number
    
*/

char* proxy_seal(char* toSeal){
	size_t sealed_size = sizeof(sgx_sealed_data_t) + sizeof(toSeal);
    uint8_t* sealed_data = (uint8_t*)malloc(sealed_size);

    sgx_status_t ecall_status, status;
    status = seal(global_id, &ecall_status,
            (uint8_t*)&toSeal, sizeof(toSeal),
            (sgx_sealed_data_t*)sealed_data, sealed_size);

    if (!is_ecall_successful(status, "Sealing failed :(", ecall_status)) {
		char* retval = "FAIL";
        return retval;
    }
    
    return (char*) sealed_data;
}

Product* createProduct(int id, char* name, int useEncrypt){
	//allocate mem for this product
	Product* retval = (Product*)malloc(sizeof(Product));
	
	//src: https://stackoverflow.com/questions/10162152/how-to-work-with-string-fields-in-a-c-struct
	// Must allocate memory for contents of pointers.  Here, strdup()
  	// creates a new copy of name.  Another option:
 	// p->name = malloc(strlen(name)+1);
  	// strcpy(p->name, name);
	//if(useEncrypt == 0){
		retval->id = id; //eq *(retval).id 
		retval->name = strdup(name);
	/*}
	else{
		int newID = id;
		char* newName = proxy_seal(name);
		if(strcmp(newID, "FAIL") + strcmp(newName, "FAIL") == 0){
			printf("Something dun fucked up");
		}
		
		retval->id = newID; //eq *(retval).id 
		retval->name = newName;
		
		
	
	}*/
	return retval;

}

Product* createProduct(int id, char* name){
	return createProduct(id, name, 0);
}

Customer* createCustomer(int id, char* name){
	return createCustomer(id, name, 0);
}

Customer* createCustomer(int id, char* name, int useEncrypt){
	//allocate mem for this product
	Customer* retval = (Customer*)malloc(sizeof(Customer));
	//if(useEncrypt == 0){
		retval->id = id; //eq *(retval).id 
		retval->name = strdup(name);
	/*}
	else{
		char* newID = id;
		char* newName = proxy_seal(name);
		if(strcmp(newID, "FAIL") + strcmp(newName, "FAIL") == 0){
			printf("Something dun fucked up");
		}
		
		retval->id = newID; //eq *(retval).id 
		retval->name = newName;
	}*/
	return retval;
}

void printProd(Product* o){
    //"id: name:"
    
    printf("id:%d name:%s\n", o->id, o->name);
	
	/*size_t len  = strlen(p->name) + strlen(p->id) + 9;
	char* retval = (char*)malloc (sizeof (char) * len);
	strcpy(retval, "id:");
	strcat(retval, p->id);
	strcat(retval, " name:");
	strcat(retval, p->name);
	
	return retval;
	*/
}

void printCust(Customer* o){
    printf("id:%d name:%s\n", o->id, o->name);
}

int gimmeID(){
	return N_IDS[rand()%5];
}
char* gimmeProd(){
	return PRODS[rand()%10];
}
char* nextName(){
	name_idx += 1; 
	return NAMES[name_idx];
}


//https://stackoverflow.com/questions/15397728/c-pointer-to-array-of-pointers-to-structures-allocation-deallocation-issues
Product** genProducts(int numProds){
	Product** arr = (Product**)malloc(numProds * sizeof(Product*));// = (Product*)malloc(sizeof(Product) * numProds);
	
	int i;
	for(i =0; i < numProds; ++i){
		arr[i] = createProduct(gimmeID(), gimmeProd(), 0);
	}
	
	return arr;
}

Customer** genCustomers(int numCusts){
	Customer** arr = (Customer**)malloc(numCusts * sizeof(Customer *));// = (Product*)malloc(sizeof(Product) * numProds);
	
	int i;
	int loop = (5 < numCusts) ? 5 : numCusts;
	for(i =0; i < loop; ++i){
		arr[i] = createCustomer(i, nextName(), 0);
	}
	
	return arr;
}
/*
srand(time(NULL));

    // allocate 100 pointers, effectively an array
    struct Test **t_array = malloc(100 * sizeof(struct Test *));

    // allocate 100 structs and have the array point to them
    for (int i = 0; i < 100; i++) {
        t_array[i] = malloc(sizeof(struct Test));
    }

    // lets fill each Test.data with a random number!
    for (int i = 0; i < 100; i++) {
        t_array[i]->data = rand() % 100;
    }

    // now define a pointer to the array
    struct Test ***p = &t_array;
    printf("p points to an array of pointers.\n"
       "The third element of the array points to a structure,\n"
       "and the data member of that structure is: %d\n", (*p)[2]->data);

    return 0;
*/



























