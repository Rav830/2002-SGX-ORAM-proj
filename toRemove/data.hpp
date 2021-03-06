/*#ifndef DATA_H
#define DATA_H

#include <stdint.h>

typedef struct{
	int id;
	char name[25];
	int expireTime;

}Customer;

typedef struct{
	int id;
	char name[50];
	int expireTime;

}Order;

void readCust(char* filename, int numLines, Customer* retval);
char* custToStr(Customer c);
void readOrder(char* filename, int numLines, Order* retval);
char* orderToStr(Order c);

uint8_t* serialize(Customer* c, Order* o, int isCust); 
void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust);

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust); 
void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust);

int isCust(uint8_t* cereal);

int hash(uint8_t* str);

#endif*/
