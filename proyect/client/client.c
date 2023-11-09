#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "../lib/lib.h"
#include "../lib/request.h"
#include "../lib/response.h"

int main(int argc, char *argv[])
{

  if (argc != 2) {
    printf("Debe de proporcionar la cadena al ejecutar el programa");
    return 1;
  }

  char *inputString = argv[1];
  sem_t *clients = sem_open(SEMAPHORE_NAME, 0);
  key_t keyForRequest = ftok(SHARED_MEMORY_FILE, 'b');
  int requestMemoryId = shmget(keyForRequest, sizeof(REQUEST), IPC_CREAT | 0777);
  REQUEST *request = (REQUEST *)shmat(requestMemoryId, NULL, 0);

  key_t keyForResponseAvailable = ftok(SHARED_MEMORY_FILE, 'z');
  int sharedMemoryReponseAvailable = shmget(
    keyForResponseAvailable, 
    sizeof(int), 
    IPC_CREAT | 0777
  );

  int *isResponseAvaible = (int *)shmat(
    sharedMemoryReponseAvailable,
    NULL,
    IPC_CREAT | 0777
  );

  key_t keyForSharedMemory = ftok(SHARED_MEMORY_FILE, 'r');
  int sharedMemoryResponse= shmget(
    keyForSharedMemory, 
    sizeof(RESPONSE),
    IPC_CREAT | 0777
  );
  RESPONSE *response = (RESPONSE *)shmat(sharedMemoryResponse, NULL, 0);

  printf("ESPERANDO CONEXION AL SERVIDOR...\n");
  sem_wait(clients);

  request->method = GET;
  strcpy(request->requestPath, inputString);

  printf("ESPERANDO RESPUESTA...\n");
  while (*isResponseAvaible != 1) {}
  
  printf("ESTATUS %d\n", response->statusCode);
  printf("PETICION RECIBIDA \n");
  return 0;
}
