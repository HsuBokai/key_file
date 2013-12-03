#include <stdio.h>
#include <stdlib.h>

#include "my_crypto.h"
#include "key_file.h"

Key_file* new_Key_file(char* path) {
#ifdef DEBUG
	fprintf(stderr, "in new_Key_file\n");
#endif
	Key_file* ret = malloc(sizeof(Key_file));
	ret->_key_file_path = path;
	return ret;
}

void delete_Key_file(Key_file* obj) {
#ifdef DEBUG
	fprintf(stderr, "in delete_Key_file\n");
#endif
	free(obj);
	obj = NULL;
}

#define TARGET "target\n"
#define PASSWORD "password\n"

int write_key_file(Key_file* obj)
{
	//if(access(key_file_path, F_OK)==0) remove(key_file_path);
	FILE* fPtr = fopen(obj->_key_file_path, "w+");
	if (!fPtr) {
		printf(" fail to open key_file!! \n");
		return -1;
	}
	fprintf(fPtr, TARGET);
	//fprintf(fPtr, "%s", target);
	fprintf(fPtr, "%s", obj->_target);
	fprintf(fPtr, "\n\n");

	fprintf(fPtr, PASSWORD);
	//char input[MAXPASSLEN];
	//memset(input, '\0', MAXPASSLEN);
	//random_pw(input, MAXPASSLEN);
	//input[MAXPASSLEN] = '\0';
	//fprintf(fPtr, "%s", input);
	int i = 0;
	for(i = 0; i<MAX_PASSWORD_SIZE; ++i)
		fprintf(fPtr, "0x%02X ", obj->_password[i]);

	fprintf(fPtr, "\n\n");

	fclose(fPtr);
	return 1;
}


int read_key_file(Key_file* obj)
{
	FILE* fPtr = fopen(obj->_key_file_path, "r");
	if (!fPtr) {
		printf(" fail to open key_file!! \n");
		return -1;
	}
	unsigned char* password = obj->_password;
	char line[64];
	//int count = 0;
	while(fgets(line, 64, fPtr)!=NULL){
		if(strcmp(line, TARGET)==0) {
			fscanf(fPtr, "%s", obj->_target);
			//fprintf(stderr, "%s\n", obj->_target); //debug
		}
		else if(strcmp(line, PASSWORD)==0) {
			int i = 0;
			for(i=0; i<MAX_PASSWORD_SIZE; ++i) {
				fscanf(fPtr, "%i", (unsigned int*)(password+i));
				//fprintf(stderr, "%02X", password[i]); //debug
			}
		}
		//fprintf(stderr, "%d\n", ++count); //debug
	}
	fclose(fPtr);
	return 0;
}

