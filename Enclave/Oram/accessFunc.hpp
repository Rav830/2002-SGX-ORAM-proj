#ifndef ACCESS_H
#define ACCESS_H

#include "storage.hpp" //cheeky way to include the structs and some functions 
//#include "../../Includconfig.h"
#include "block.hpp" //for the block functions
#include "bucket.hpp" // for the bucket functions 

#include "../../Include/oramStructs.hpp"
//#include <math.h>

int rdRand(void);
int lcg(void);
int rand(void);
int getpmID(StorageManager* oramSM, int bid);

//return the largest int where a tree would match indicies
//@return a valid index for the bucket array to set
int intersect_idx(int leafOne, int leafTwo, int height, int numBuckets, int leaves);


Block access(Storage* oram, int op, Block* data, int pmID, StorageManager* oramSM);

#endif
