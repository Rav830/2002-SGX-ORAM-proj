#include "config.h"
#include "block.h"
#include "bucket.h"
#include "storage.h"

#include <stdio.h>

void testcode(){

	char d[MAX_BLOCK_SIZE];
	
	int i;
	for(i=0; i<MAX_BLOCK_SIZE-1; ++i){
		d[i] = i+96-32;
	
	}
	d[MAX_BLOCK_SIZE-1] = '\0';
	
	
	Block b = create_block(5, d);
	
	
	
	printf("%d %s\n", b.bid, b.data);
	
	Block dummy = create_dummy_block();
	
	printf("%d %s\n", dummy.bid, dummy.data);
	
	
	Bucket dumdum = create_dummy_bucket();
	print_bucket(dumdum);

	printf("Testing place block\n");
	int retval = place_block(b, &dumdum);
	printf("place_block status %d\n", retval);
	print_bucket(dumdum);

	Storage test = create_storage();
	
	print_storage(test);


}

int main(){

	

	printf("compile\n");
	
	testcode();
	
	
	return 1;
}


