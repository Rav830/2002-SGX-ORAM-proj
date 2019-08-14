#include "../Include/dataStruct.hpp"
#include "dataFunc.hpp"
#include <stdio.h>
#include <time.h>

int numCusts = 0, numOrders = 0, cnt = 0;
//assume the cust and order buffers are empty
int NLJ(Customer* custs, Order* ords, uint8_t** inputArr, int inputLen, int windowSeconds, bool printMatch){

	//Assume Customer is smaller
	//and just print the contents 
	int i, j;
	int currentTime = (int)time(NULL);
	Customer tmpC;
	Order tmpO;
	int isCustStore = 0;
	for(i = 0; i< inputLen; i++){
		//for each tuple
		//printf("Current Time: %d\n", currentTime);
		isCustStore = isCust(inputArr[i]);
		deserialize(inputArr[i], &tmpC, &tmpO, isCustStore);

		
		if(isCustStore){
			//is customer check the orders for a match and don't match on an expired tuple
					//spin lock until the the time is correct
		
			for(j=0; j<numOrders; j++){
				currentTime = (int)time(NULL);
				//the code that handles eviction will replace the current tuple with the last one 
				if(ords[j].expireTime < currentTime - windowSeconds){
					ords[j] = ords[numOrders-1];
					numOrders--;
					j--;
				}
				else {
					if(tmpC.id == ords[j].id &&  !(ords[j].expireTime < currentTime - windowSeconds)){
						cnt++;
						if(printMatch){
							printf("Match: %s, %s\n", custToStr(tmpC), orderToStr(ords[j]));
						}
					}
				}
			}
			
			//After join add to the array
			custs[numCusts] = tmpC;
			numCusts++;
			
		}
		else{
			//is order check the customers for a match and don't match on an expired tuple
			for(j=0; j<numCusts; j++){
				currentTime = (int)time(NULL);
				//evict check
				if(custs[j].expireTime < currentTime - windowSeconds){
					custs[j] = custs[numCusts-1];
					numCusts--;
					j--;
				}
				else{
					if(tmpO.id == custs[j].id && !(custs[j].expireTime < currentTime - windowSeconds) ){
						cnt++;
						if(printMatch){
							printf("Match: %s, %s\n", custToStr(custs[j]), orderToStr(tmpO));
						}
					}
				}
			}
			
			//After join add to the array
			ords[numOrders] = tmpO;
			numOrders++;		
		
		}
	}
	
	//printf("Num Joins is: %d\n", cnt); 


	return cnt;
}
