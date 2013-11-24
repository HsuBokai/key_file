#include <stdio.h>
#include <stdlib.h>

#include "key_file.h"

int main(int argc, char *argv[])
{
	printf("helloworld is running !!! \n");

	char path[] = "/home/bokai/key_file/testfile";

	Key_file* obj = new_Key_file( path );

	int i = 0;
	for(i = 0; i<1; ++i)
	{
		Key_file obj2 = { ._key_file_path=path, ._target="hello", ._password="abc" };
		write_key_file(&obj2);
		read_key_file(obj);
	for(i = 0; i<MAX_PASSWORD_SIZE; ++i)
		if(obj->_password[i]!=obj2._password[i]) fprintf(stderr, "fail\n");
	}
	delete_Key_file(obj);
	return 0;
}
