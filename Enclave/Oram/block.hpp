
#ifndef BLOCK_H
#define BLOCK_H
#include "../../Include/oramStructs.hpp"
//#include "config.h"
//#include <stdint.h>

//a block is an array of data with a max size from the config

//data is the content of the block

/*typedef struct{
	//int bid;
	uint8_t data[MAX_BLOCK_SIZE];

} Block; 
*/
//creates a block with a bid data set to all ones 
Block create_block(uint8_t bid, uint8_t data[]); 
Block create_dummy_block();
void print_block(Block toPrint);


#endif //BLOCK_H
