#ifndef ENCDATA_H
#define ENCDATA_H

#include "../Include/config.h"
#include "../Include/dataStruct.hpp"
#include <stdint.h>
//void readCust(char* filename, int numLines, Customer* retval);
//void readOrder(char* filename, int numLines, Order* retval);

char* orderToStr(Order c);
char* custToStr(Customer c);


void uint32_to_uint8(int from, uint8_t* to);
int uint8_to_uint32(uint8_t* in);

uint8_t* serialize(Customer* c, Order* o, int isCust); 
void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust);

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust); 
void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust);

int isCust(uint8_t* cereal);

uint8_t hash(uint8_t* str);

#endif
