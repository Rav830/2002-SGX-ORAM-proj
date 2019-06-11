#ifndef ORAM_CONFIG
#define ORAM_CONFIG

#define MAX_BLOCK_SIZE 961
#define MAX_BUCKET_SIZE 4
#define INIT_STORAGE_ELEMS 8
#define INIT_STORAGE_SIZE 15
#define HASH_RANGE 8
/*
Some info about the above defines
Max_block_size determines the number of tuples can exist in one block,
	to determine the number of tuples one may want in a block, take the current tuple size * number of elems and add one for keeping track of the hash

Max_bucket_size determines the number of blocks that can fit in a particular bucket

Init_storage_elems is the number of elements we can effectively store in this tree this is the number of leaves in a full binary tree at a particular depth
Init_storage_size is just the number of nodes in a full binary tree at the same size as the one used in the last variable

Hash_range is just a helper to determine the hash values that should be produced from the hashing function



*/
#define STASH_SIZE 20
#define PM_SIZE 10
#define ORAM_READ 0
#define ORAM_WRITE 1
#define ORAM_APPEND 2

#endif 
