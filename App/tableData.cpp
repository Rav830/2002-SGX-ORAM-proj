#include "../Include/tableData.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char IDS[10][2] = {"1", "2", "3","4","5","6","7","8", "9", "0"};
char NAMES[5][10] = {"Townie","Simonette" , "Dinnie", "Merell", "Madelon"};
char PRODS[10][20] = {"Jolt Cola", "Breadfruit", "Cream Soda", "Corn", "Gatorade", "Lettuce",  "Bib", "Bread", "Dinner", "Chicken"};

Product* createProduct(char* id, char* name){
	//allocate mem for this product
	Product* retval = (Product*)malloc(sizeof(Product));
	
	//src: https://stackoverflow.com/questions/10162152/how-to-work-with-string-fields-in-a-c-struct
	// Must allocate memory for contents of pointers.  Here, strdup()
  	// creates a new copy of name.  Another option:
 	// p->name = malloc(strlen(name)+1);
  	// strcpy(p->name, name);
	retval->id = strdup(id); //eq *(retval).id 
	retval->name = strdup(name);
	
	return retval;

}

Customer* createCustomer(char* id, char* name){
	//allocate mem for this product
	Customer* retval = (Customer*)malloc(sizeof(Customer));
	retval->id = strdup(id); //eq *(retval).id 
	retval->name = strdup(name);
	
	return retval;
}

void printProd(Product* o){
    //"id: name:"
    
    printf("id:%s name:%s\n", o->id, o->name);
	
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
    printf("id:%s name:%s\n", o->id, o->name);
}

char* gimmeID(){
	return IDS[rand()%10];
}
char* gimmeProd(){
	return PRODS[rand()%10];
}
char* gimmeName(){
	return NAMES[rand()%5];
}


//https://stackoverflow.com/questions/15397728/c-pointer-to-array-of-pointers-to-structures-allocation-deallocation-issues
Product** genProducts(int numProds){
	Product** arr = (Product**)malloc(numProds * sizeof(Product *));// = (Product*)malloc(sizeof(Product) * numProds);
	
	int i;
	for(i =0; i < numProds; ++i){
		arr[i] = createProduct(gimmeID(), gimmeProd());
	}
	
	 Product ***retval = &arr;
	return arr;
}

Customer** genCustomers(int numProds){
	Customer** arr = (Customer**)malloc(numProds * sizeof(Customer *));// = (Product*)malloc(sizeof(Product) * numProds);
	
	int i;
	for(i =0; i < numProds; ++i){
		arr[i] = createCustomer(gimmeID(), gimmeName());
	}
	
	//Customer*** retval = &arr;
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



























