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
#include "../lib/entities.h"
#include "controllers.h"

int isUserAuthenticated = 0;
User userCurrentlyAuth;

int isBadResponse(STATUS status)
{
  int bad = 0;
  switch (status)
  {
  case BAD_REQUEST:
  case INTERNAL_ERROR:
  case UNAUTHORIZED:
  case NOT_FOUND:
    bad = 1;
    break;
  }
  return bad;
}

void manageResponse(char route[256])
{
  
  key_t keyForSharedMemory = ftok(SHARED_MEMORY_FILE, 'r');
  int sharedMemoryId = shmget(
      keyForSharedMemory,
      sizeof(RESPONSE),
      IPC_CREAT | 0777);
  RESPONSE *response = (RESPONSE *)shmat(sharedMemoryId, NULL, 0);

  FILE *body = fopen("../shared/body.txt", "rb");
  Error error;

  if (isBadResponse(response->statusCode))
  {
    fread(&error, sizeof(Error), 1, body);
    printf("Mensaje: %s", error.message);
  }
  else
  {
    if (strcmp(route, loginRoute) == 0 || strcmp(route, registerRoute) == 0)
    {

      fread(&userCurrentlyAuth, sizeof(User), 1, body);
      isUserAuthenticated = 1;
      printf("USUARIO AUTENTICADO %s", userCurrentlyAuth.id);
    }

    if (strcmp(route, getAllAuctionsRoute) == 0)
    {
      Auction readAuction;
      printf("SUBASTAS DISPONIBLES \n");
      while (fread(&readAuction, sizeof(Auction), 1, body) == 1)
      {
        printf("\n");
        printf("ID: %s \n", readAuction.id);
        printf("TITULO: %s \n", readAuction.title);
        printf("PUJA ACTUAL: %f \n", readAuction.actualBid);
        printf("VENDEDOR: %s \n", readAuction.sellerName);
        printf("\n");
      }
      fclose(body);
    }

    if (strcmp(route, bidToAuction) == 0)
    {
      printf("HAZ HECHO TU OFERTA\n");
      fclose(body);
    }
  }
}

void waitForAnswer()
{
  key_t keyForResponseAvailable = ftok(SHARED_MEMORY_FILE, 'z');
  int sharedMemoryReponseAvailable = shmget(
      keyForResponseAvailable,
      sizeof(int),
      IPC_CREAT | 0777);

  int *isResponseAvaible = (int *)shmat(
      sharedMemoryReponseAvailable,
      NULL,
      IPC_CREAT | 0777);

  printf("\nESPERANDO RESPUESTA...\n");
  while (*isResponseAvaible != 1)
  {
  }
  *isResponseAvaible = 0;
}

int isAuthenticated()
{
  if (isUserAuthenticated == 0)
  {
    printf("Autenticacion necesaria\n");
    return 0;
  }

  return 1;
}

int main(int argc, char *argv[])
{

  sem_t *clients = sem_open(SEMAPHORE_NAME_CLIENTS, 0);
  key_t keyForRequest = ftok(SHARED_MEMORY_FILE, 'b');
  int requestMemoryId = shmget(keyForRequest, sizeof(REQUEST), IPC_CREAT | 0777);
  REQUEST *request = (REQUEST *)shmat(requestMemoryId, NULL, 0);
  printf("ESPERANDO CONEXION AL SERVIDOR...\n");

  int keepMenu = 1;
  int choice;

  do
  {
    printf("\n");
    printf("peticiones: \n");
    printf("%s [1] POST \n", registerRoute);
    printf("%s [2] POST\n", loginRoute);
    printf("%s [3] POST \n", createAuction);
    printf("%s [4] GET\n", getAllAuctionsRoute);
    printf("%s [5] PUT\n", bidToAuction);
    printf("\n");

    printf("OPCION: ");
    scanf("%d", &choice);
    printf("\n");
    FILE *body = fopen("../shared/body.txt", "wb");
    switch (choice)
    {
    case 1:
      request->method = POST;
      strcpy(request->requestPath, registerRoute);
      User user = registerUserController();
      sem_wait(clients);
      if (body == NULL)
      {
        printf("Error al abrir el archivo.\n");
        return INTERNAL_ERROR;
      }

      int flag = 0;
      flag = fwrite(&user, sizeof(User), 1, body);
      fclose(body);
      waitForAnswer();
      manageResponse(request->requestPath);
      break;
    case 2:
      request->method = POST;
      strcpy(request->requestPath, loginRoute);
      Login login = loginUserController();
      sem_wait(clients);
      if (body == NULL)
      {
        printf("Error al abrir el archivo.\n");
        return INTERNAL_ERROR;
      }

      fwrite(&login, sizeof(Login), 1, body);
      fclose(body);
      waitForAnswer();
      manageResponse(request->requestPath);
      break;
    case 3:
      if (!isAuthenticated())
        break;
      request->method = POST;
      strcpy(request->requestPath, createAuction);
      Auction auction = createAuctionController(userCurrentlyAuth);
      sem_wait(clients);
      if (body == NULL)
      {
        printf("Error al abrir el archivo.\n");
        return INTERNAL_ERROR;
      }

      fwrite(&auction, sizeof(Auction), 1, body);
      fclose(body);
      waitForAnswer();
      manageResponse(request->requestPath);
      break;
    case 4:
      if (!isAuthenticated())
        break;
      request->method = POST;
      strcpy(request->requestPath, getAllAuctionsRoute);
      sem_wait(clients);
      waitForAnswer();
      manageResponse(request->requestPath);
      break;
    case 5:
      if (!isAuthenticated())
        break;
      request->method = PUT;
      strcpy(request->requestPath, bidToAuction);
      Bid bid = bidToAuctionController(userCurrentlyAuth);
      sem_wait(clients);
      if (body == NULL)
      {
        printf("Error al abrir el archivo.\n");
        return INTERNAL_ERROR;
      }

      fwrite(&bid, sizeof(Bid), 1, body);
      fclose(body);
      waitForAnswer();
      manageResponse(request->requestPath);
      break;
    default:
      break;
    }
    printf("\n");
  } while (keepMenu);

  return 0;
}
