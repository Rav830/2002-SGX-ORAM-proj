
void print_storage(Storage* outside){
	int i, j,k;
	for(k = 0; k< INIT_STORAGE_SIZE; k++){
		printf("Bucket %d\n", k);
		for (j = 0; j<MAX_BUCKET_SIZE; j++){
			printf("Block %d\n", j);
			for(i = 0; i<MAX_BLOCK_SIZE; i++){
				printf("%d ", outside->allBuckets[k].blocks[j].data[i]);
			}
			printf("\n");
		}
	}

}


int generate_random_number() {
    ocall_println("Processing random number generation...");
    return 42;
}
void storage_test(Storage* outside){
	//Storage tmp = create_storage();
	init_storage(outside);
	
}

void access_func_test(Storage* outside){
	init_storage(outside);
	//printf("%d\n", rand());
	StorageManager outsideSM = create_manager();
	Block toEnter;
	memset(toEnter.data, 5, MAX_BLOCK_SIZE);
	int pm_loc = add_bid(&outsideSM, toEnter.data[0]);
	printf("Location: %d\n", pm_loc);
	printf("outsideSM idx: %d\n", outsideSM.stashIDX);	 
	//printf("(hash, pm, loc) (%d, %d, %d)\n", toEnter.data[0], pm_loc, oramSM->pmIDX[pm_loc]);
	//run a check for a failed add_bid
	if(pm_loc+1){
		//fprintf("trying access\n");
		access(outside, ORAM_WRITE, &toEnter, pm_loc, &outsideSM);
	}
	else{
		printf("Add Tuple failed\n");

	}
	
	print_storage(outside);
	
		

}


void encryption_test(){

	//set the key for encryption
	uint8_t p_key[16];
	memset(p_key, 1, 16);
	//p_key[0] = 1;
	
	//set the data to be encrpyted
	char text_src[] = "Text To Encrypt";
	//char text_src[15];
	//memset(text_src, 4, 15);
	uint32_t text_len = strlen(text_src);
	
	
	//initialization vector
	uint32_t iv_len = 12; 
	uint8_t iv[iv_len];
	memset(iv, 3, iv_len);
	
	uint32_t aad_len = 0;
	const uint8_t* aad = NULL;
	
	//output
	char* text_encrypt = (char*)malloc(text_len*sizeof(char));
	uint8_t p_out_mac[16];
	memset(p_out_mac, 0, 16);
	int i;
	
	printf("Before Encryption details\n");
	
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	
	printf("Encrypting...\n");
	
	sgx_status_t retval = sgx_rijndael128GCM_encrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_src, text_len, (uint8_t*)text_encrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval==SGX_ERROR_INVALID_PARAMETER);
	
	printf("After Encryption Details\n");
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	
	printf("Decrypting....\n");
	char text_decrypt[text_len];
	retval = sgx_rijndael128GCM_decrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_encrypt, text_len, (uint8_t*)text_decrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval);
	printf("\t Dencrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", text_decrypt[i]);
	}
	printf("\n");

	
	printf("Encrypting...\n");
	memset(iv, 4, iv_len);
	retval = sgx_rijndael128GCM_encrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_src, text_len, (uint8_t*)text_encrypt, iv, iv_len, NULL, 0, (sgx_aes_gcm_128bit_tag_t*)p_out_mac);
	
	printf("SGX_Status: %d\n", retval);
	
	printf("After Encryption Details\n");
	printf("\t Encryption Key: {");

	for(i=0; i<16; i++){
		printf("%d, ", p_key[i]);
	}
	printf("}\n");
	
	//printf("\t Source Text: %s\n", text_src);
	printf("\t Source Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", ((uint8_t*)text_src)[i]);
	}
	printf("\n");
	printf("\t Source Length: %d\n", text_len);
	
	printf("\t Init Vector: {");
	for(i=0; i<iv_len; i++){
		printf("%d, ", iv[i]);
	}
	printf("}\n");
	
	printf("\t aad: {");
	for(i=0; i<aad_len; i++){
		printf("%d, ", aad[i]);
	}
	printf("}\n");
	
	printf("\t Encrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%d ", text_encrypt[i]);
	}
	printf("\n");
	
	printf("\t MAC: {");
	for(i=0; i<16; i++){
		printf("%d, ", p_out_mac[i]);
	}
	printf("}\n");
	printf("Decrypting....\n");
	char text_decrypt_two[text_len];
	memset(p_key, 5, 16);
	retval = sgx_rijndael128GCM_decrypt((sgx_aes_gcm_128bit_key_t*)p_key, (uint8_t*)text_encrypt, text_len, (uint8_t*)text_decrypt_two, iv, iv_len, NULL, 0, NULL);
	
	printf("SGX_Status: %d\n", retval== SGX_ERROR_MAC_MISMATCH);
	printf("\t Dencrypted Text: ");
	for(i=0; i<text_len; i++){
		printf("%c", text_decrypt_two[i]);
	}
	printf("\n");
	
	
	

}

void rand_test(){
	printf("Creating an array of 10 random numbers\n");
	uint8_t buffer[12];
	memset(buffer, 0, 12);
	sgx_read_rand(buffer, 12);
	int i;
	for(i=0; i<12; i++){
		printf("%d ", buffer[i]);
	}
	printf("\n");


	printf("Creating a single random number (32 bit signed) (doing it 10 times)\n");
	int num = 0;
	for(i=0; i<10; i++){
		sgx_read_rand((unsigned char*) &num, 4);
		printf("%d\n", num);
	}
}

void enclave_fill_mem(){
	ocall_print("Setting Memory\n");
	uint8_t fillMem[128*5000];
	ocall_print("Filling Memory\n");
	int i;
	//for(i=0; i<128*2000; i++){
	//	fillMem[i] = (uint8_t)120;
	//}
	memset(fillMem, 133, 128*5000);



}

void ecall_testStruct(testB* obj){
	printf("==> Entering enclave\n");
	printf("c: %d\n", obj->c);
	
	testA data;
	data.a = 1;
	data.b = 5;
	
	obj->d = data;
	
	printf("a: %d\n", obj->d.a);
	printf("b: %d\n", obj->d.b);
	
	printf("B Addr: %p\n", (void*)obj);
	printf("A Addr: %p\n", (void*)&(obj->d));
	printf("data Addr: %p\n", (void*)&data);
	
	printf("<== Exiting enclave\n");
}

/*



/*
void ecall_print_block(Block* root){
	ocall_print("==> in enclave");
	ocall_print("Root info");
	ocall_print_int(root->id);
	ocall_print(root->content);
	ocall_print("Left info");
	ocall_print_int(root->left->id);
	ocall_print(root->left->content);
	ocall_print("Right info");
	ocall_print_int(root->right->id);
	ocall_print(root->right->content);
	ocall_print("leftleft info");
	ocall_print_int(root->left->left->id);
	ocall_print(root->left->left->content);	
	ocall_print("<== Exiting enclave");
}*/
