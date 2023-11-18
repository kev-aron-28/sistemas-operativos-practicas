#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "../lib/lib.h"
#include "../lib/request.h"
#include "../lib/response.h"
#include "../lib/entities.h"
#include "controllers.h"

void *routing(void *args)
{
  REQUEST *request = (REQUEST *)args;

  key_t keyForSharedMemory = ftok(SHARED_MEMORY_FILE, 'r');
  int sharedMemoryId = shmget(
    keyForSharedMemory, 
    sizeof(RESPONSE),
    IPC_CREAT | 0777
  );
  RESPONSE *response = (RESPONSE *)shmat(sharedMemoryId, NULL, 0);
  response->statusCode = NOT_FOUND;
  response->hasBody = 0;

  if(strcmp(request->requestPath, registerRoute) == 0) {
    STATUS responseController = registerUserController();
    response->statusCode = responseController;
    response->hasBody = 1;
  }

  if(strcmp(request->requestPath, loginRoute) == 0) {
    STATUS responseController = loginUserController();
    response->statusCode = responseController;
    response->hasBody = 1;
  }

  if(strcmp(request->requestPath, createAuction) == 0) {
    STATUS responseController = createAuctionController();
    response->statusCode = responseController;
    response->hasBody = 1;
  }

  if(strcmp(request->requestPath, getAllAuctionsRoute) == 0) {
    STATUS responseController = getAllAuctionsController();
    response->statusCode = responseController;
    response->hasBody = 1;
  }

  if(strcmp(request->requestPath, bidToAuction) == 0) {
    STATUS responseController = bidToAuctionController();
    response->statusCode = responseController;
    response->hasBody = 1;
  }
}

int main()
{
  sem_t *clients = sem_open(SEMAPHORE_NAME_CLIENTS, O_CREAT | O_EXCL, 0666, 1);
  sem_t *server = sem_open(SEMAPHORE_NAME_SERVER, O_CREAT | O_EXCL, 0666, 1);
  key_t keyForRequest = ftok(SHARED_MEMORY_FILE, 'b');
  
  int sharedMemoryRequest = shmget(
    keyForRequest, 
    sizeof(REQUEST), 
    IPC_CREAT | 0777
  );

  REQUEST *request = (REQUEST *)shmat(
    sharedMemoryRequest, 
    NULL, 
    0
  );

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
  *isResponseAvaible = 0;

  if (request == (REQUEST *)(-1) || isResponseAvaible == (int *)(-1))
  {
    perror("Error al adjuntar la memoria compartida");
  }

  if (clients == SEM_FAILED)
  {
    if (errno == EEXIST)
    {
      clients = sem_open(SEMAPHORE_NAME_CLIENTS, 0);
    }
    else
    {
      perror("sem_open");
      sem_unlink(SEMAPHORE_NAME_CLIENTS);
      return 1;
    }
  }

  if (clients == SEM_FAILED)
  {
    if (errno == EEXIST)
    {
      clients = sem_open(SEMAPHORE_NAME_CLIENTS, 0);
    }
    else
    {
      perror("sem_open");
      sem_unlink(SEMAPHORE_NAME_CLIENTS);
      return 1;
    }
  }

  int currentValue;
  if (sem_getvalue(clients, &currentValue) == -1)
  {
    perror("sem_getvalue");
    exit(1);
  }

  do
  {
    pthread_t routingThread;
    if (currentValue == 0)
    {
      printf("\n");
      printf("CONEXION AL SERVIDOR...\n");
      pthread_create(&routingThread, NULL, routing, request);
      pthread_join(routingThread, NULL);
      sem_post(clients);
      *isResponseAvaible = 1;
      printf("CLIENTE DESPACHADO...\n");
      printf("\n");
    }
    sem_getvalue(clients, &currentValue);
  } while (1);

  return 0;
}
