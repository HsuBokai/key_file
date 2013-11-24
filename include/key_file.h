#ifndef KEY_FILE_H
#define KEY_FILE_H 1

#define MAX_TARGET_SIZE 8
#define MAX_PASSWORD_SIZE 28

typedef struct _Key_file	Key_file;

struct _Key_file {
	char* _key_file_path;
	unsigned char _target[MAX_TARGET_SIZE];
	unsigned char _password[MAX_PASSWORD_SIZE];
};

Key_file* new_Key_file();
void delete_Key_file(Key_file* obj);

int read_key_file(Key_file* obj);
int write_key_file(Key_file* obj);
#endif
