#ifndef LIB_H
#define LIB_H

#define SHARED_MEMORY_FILE "../shared/sharedMemory.txt"
#define JSON_BODY_FILE "../shared/body.txt"
#define SEMAPHORE_NAME_CLIENTS "/clients_semaphore_test"
#define SEMAPHORE_NAME_SERVER "/server_semaphore"
#define USER_FILE "../shared/users.txt"
#define KEY_ENCRYPT 10

void encryptPassword(char password[]);
int comparePasswords(char passwordInput[], char passwordSaved[]);


#endif
