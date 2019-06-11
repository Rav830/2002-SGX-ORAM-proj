///////////////////////
// Currently Unused
//////////////
#include "../../Include/tableData.h"
#include <stdio.h>


int NLJ(Product** prodArr, int prodLen, Customer** custArr, int custLen){

	printf("join content\nid, Customer, Product\n");
	//Assume Customer is smaller
	//and just print the contents 
	int i, j;
	
	for(i = 0; i< custLen; ++i){
		
		for(j=0; j<prodLen; ++j){
			
			//check if the id's match
			if(custArr[i]->id == prodArr[j]->id){
				printf("%d, %s, %s\n", custArr[i]->id, custArr[i]->name, prodArr[j]->name);
			}
		}
	} 


	return 1;
}
