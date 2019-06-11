#ifndef APPDATA_H
#define APPDATA_H

#include "../Include/config.h"
#include "../Include/dataStruct.hpp"
#include <stdint.h>

void readCust(char* filename, int numLines, Customer* retval);
void readOrder(char* filename, int numLines, Order* retval);

char* orderToStr(Order c);
char* custToStr(Customer c);

uint8_t* serialize(Customer* c, Order* o, int isCust); 
void deserialize(uint8_t* cereal, Customer* c, Order* o, int isCust);

uint8_t* serializeWithHash(Customer* c, Order* o, int isCust); 
void deserializeWithHash(uint8_t* cereal, Customer* c, Order* o, int isCust);

int isCust(uint8_t* cereal);

int hash(uint8_t* str);

#endif
