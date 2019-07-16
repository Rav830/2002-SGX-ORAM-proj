#ifndef DATA_H
#define DATA_H

#include <stdint.h>

typedef struct{
	int id;
	uint8_t expireTime;
	char name[25];
	

}Customer;

typedef struct{
	int id;
	uint8_t expireTime;
	char name[50];
	

}Order;

void readCust(char* filename, int numLines, Customer* retval);
char* custToStr(Customer c);
void readOrder(char* filename, int numLines, Order* retval);
char* orderToStr(Order c);


void uint32_to_uint8(int from, uint8_t* to);
int uint8_to_uint32(uint8_t* in);

uint8_t* serialize(Customer* c, Order* o, int isCust); 
void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust);

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust); 
void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust);

int isCust(uint8_t* cereal);

int hash(uint8_t* str);

#endif
