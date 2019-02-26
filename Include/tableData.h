#include "sgx_urts.h"

#ifndef TABLEDATA_H
#define TABLEDATA_H

typedef struct{
	char *name;
	char *id;

} Customer;

typedef struct{
	char *name;
	char *id;

} Product;

void setGlobalID(sgx_enclave_id_t input);


Product* createProduct(char* id, char* name, int useEncrypt);
Customer* createCustomer(char* id, char* name, int useEncrypt);
Product* createProduct(char* id, char* name);
Customer* createCustomer(char* id, char* name);
void printProd(Product* s);
void printCust(Customer* s);

Product** genProducts(int numProds);
Customer** genCustomers(int numCusts);

char* gimmeID(void);
char* gimmeProd(void);
char* gimmeName(void);

#endif //TABLEDATA_H
