#include "sgx_urts.h"

#ifndef TABLEDATA_H
#define TABLEDATA_H

typedef struct{
	int id;
	char *name;
} Customer;

typedef struct{
	int id;
	char *name;

} Product;

/*
typedef struct Block Block;

struct Block{
	int id;
	char* content;
	Block* left;
	Block* right;

};

*/
void setGlobalID(sgx_enclave_id_t input);


Product* createProduct(int id, char* name, int useEncrypt);
Customer* createCustomer(int id, char* name, int useEncrypt);
//Product* createProduct(char* id, char* name);
//Customer* createCustomer(char* id, char* name);
void printProd(Product* s);
void printCust(Customer* s);

Product** genProducts(int numProds);
Customer** genCustomers(int numCusts);

int gimmeID(void);
char* gimmeProd(void);
char* gimmeName(void);

#endif //TABLEDATA_H
