#include "tableData.h"


//make sure the data is allocated before writing to it
void serialize_prod(Product* prod, char* data){
	
	//store the ints
	int* retval = (int*)data;
	*retval = prod->id;
	retval++;
	
	//store the name
	
	//loop until null char
	char* c = (char*)retval;
	int i = 0;
	while( prod->name[i] != '\0' ){
		*c = prod->name[i];
		c++;
	}

}


void deserialize_prod(char* data, Product* prod){
	int* p = (int*)data;
	
	prod->id = *p;
	p++;
	
	int len = 0;
	
	

}
