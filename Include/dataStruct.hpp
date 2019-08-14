#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <stdint.h>
typedef struct{
	int id;
	int expireTime;
	char name[25];

}Customer;

typedef struct{
	int id;
	int expireTime;
	char name[50];


}Order;

typedef struct{
	int a;
	int b;
}testA;

typedef struct{
	int c;
	testA d;
}testB;

#endif
