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
int main(int argc, char *argv[])
{

  if(argc != 2) {
    printf("Debe de proporcionar la cadena al ejecutar el programa");
    return 1;
  }

  char *inputString = argv[1]; 
  const char *semaphore_clients = "/client_semaphore";
  sem_t *clients = sem_open(semaphore_clients, 0);
  key_t keyForSharedMemory = ftok("token", 'a');
  int sharedMemoryId = shmget(keyForSharedMemory, sizeof(char) * 50, IPC_CREAT | 0777);
  char *clientData = (char *)shmat(sharedMemoryId, NULL, 0);

  if (clientData == (char *)(-1)) {
    perror("Error al adjuntar la memoria compartida");
  }

  printf("ESPERANDO CONEXION...\n");
  sem_wait(clients);
  strcpy(clientData, inputString);
  printf("PETICION RECIBIDA \n");
  shmdt(clientData);
  return 0;
}
