#ifndef DATASTRUCT_H
#define DATASTRUCT_H

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

typedef struct{
	int a;
	int b;
}testA;

typedef struct{
	int c;
	testA d;
}testB;

#endif
