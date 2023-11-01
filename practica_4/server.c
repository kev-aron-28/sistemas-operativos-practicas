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

// Primero se ejecuta el servidor y despues el cliente

void *thread(void *args)
{
  printf("\n");
  char *clientString = (char *)args;
  pthread_t tid = pthread_self();
  printf("HILO CON ID: %ld\n", tid);
  printf("Cadena recibida %s\n", clientString);
  sleep(5);
  printf("\n");
}

int main()
{
  const char *semaphore_clients = "/client_semaphore";
  sem_t *clients = sem_open(semaphore_clients, O_CREAT | O_EXCL, 0666, 1);
  key_t keyForSharedMemory = ftok("token", 'a');
  int sharedMemoryId = shmget(keyForSharedMemory, sizeof(char) * 50, IPC_CREAT | 0777);
  char *clientData = (char *)shmat(sharedMemoryId, NULL, 0);

   if (clientData == (char *)(-1)) {
    perror("Error al adjuntar la memoria compartida");
  }


  if (clients == SEM_FAILED)
  {
    if (errno == EEXIST)
    {
      clients = sem_open(semaphore_clients, 0);
    }
    else
    {
      perror("sem_open");
      sem_unlink(semaphore_clients);
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
    pthread_t threadForUser;
    if (currentValue == 0)
    {
       printf("\n");
      printf("CONEXION AL SERVIDOR...\n");
      pthread_create(&threadForUser, NULL, thread, clientData);
      pthread_join(threadForUser, NULL);
      sem_post(clients);
      printf("CLIENTE DESPACHADO...\n");
       printf("\n");
    }
    sem_getvalue(clients, &currentValue);
  } while (1);

  return 0;
}
