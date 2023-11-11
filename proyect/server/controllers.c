#include <stdio.h>
#include "../lib/entities.h"
#include "../lib/response.h"
#include "../lib/lib.h"
#include "controllers.h"
#include <string.h>
#include <uuid/uuid.h>

STATUS registerUserController()
{

  FILE *body = fopen("../shared/body.txt", "rb");
  User userToCreate;
  int isBodyOk = fread(&userToCreate, sizeof(User), 1, body);
  if (isBodyOk == 0)
  {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    return BAD_REQUEST;
  }
  fclose(body);

  FILE *usersFile = fopen("../shared/users.txt", "rb");
  int userAlreadyExists;
  User readUser;
  if (usersFile == NULL)
  {
    Error error = {"Error en el servidor"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    return INTERNAL_ERROR;
  }

  while (fread(&readUser, sizeof(User), 1, usersFile) == 1)
  {
    if (strcmp(readUser.email, userToCreate.email) == 0)
    {
      Error error = {"El usuario ya existe"};
      body = fopen("../shared/body.txt", "wb");
      fwrite(&error, sizeof(Error), 1, body);
      fclose(body);
      return BAD_REQUEST;
    }
  }
  fclose(usersFile);

  usersFile = fopen("../shared/users.txt", "ab");
  if (usersFile == NULL)
  {
    Error error = {"Error en el servidor"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    return INTERNAL_ERROR;
  }
  uuid_t userId;
  uuid_generate(userId);

  uuid_unparse(userId, userToCreate.id);
  int opSuccessful = 0;
  opSuccessful = fwrite(&userToCreate, sizeof(User), 1, usersFile);
  fclose(usersFile);
  if (opSuccessful)
  {
    body = fopen("../shared/body.txt", "wb");
    fwrite(&userToCreate, sizeof(User), 1, body);
    return OK;
  }

  Error error = {"Error en el servidor"};
  body = fopen("../shared/body.txt", "wb");
  fwrite(&error, sizeof(Error), 1, body);
  return INTERNAL_ERROR;
}

STATUS loginUserController()
{

  FILE *body = fopen("../shared/body.txt", "rb");
  Login login;
  int isBodyOk = fread(&login, sizeof(Login), 1, body);

  if (isBodyOk == 0)
  {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    return BAD_REQUEST;
  }
  fclose(body);

  FILE *usersFile = fopen("../shared/users.txt", "rb");
  int userToCompare;
  User readUser;
  if (usersFile == NULL)
  {
    Error error = {"Error del servidor"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return INTERNAL_ERROR;
  }

  while (fread(&readUser, sizeof(User), 1, usersFile) == 1)
  {
    if (strcmp(readUser.email, login.email) == 0 && strcmp(readUser.password, login.password) == 0)
    {
      body = fopen("../shared/body.txt", "wb");
      fwrite(&readUser, sizeof(User), 1, body);
      fclose(body);
      return OK;
    }
  }
  fclose(usersFile);

  Error error = {"Credenciales incorrectas"};
  body = fopen("../shared/body.txt", "wb");
  fwrite(&error, sizeof(Error), 1, body);
  fclose(body);

  return UNAUTHORIZED;
}

STATUS createAuction()
{
  FILE *body = fopen("../shared/body.txt", "rb");
  Auction auctionToCreate;
  int isBodyOk = fread(&auctionToCreate, sizeof(Auction), 1, body);
  
  if (isBodyOk == 0) {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    return BAD_REQUEST;
  }

  FILE *auctionsFile = fopen("../shared/auctions.txt", "ab");

  fclose(body);
  uuid_generate(auctionToCreate.id);
  
  int opSuccessful = 0;
  opSuccessful = fwrite(&auctionToCreate, sizeof(Auction), 1, auctionsFile);

  fclose(auctionsFile);

  if (opSuccessful)
  {
    body = fopen("../shared/body.txt", "wb");
    fwrite(&auctionToCreate, sizeof(Auction), 1, body);
    return OK;
  }

  Error error = {"Error en el servidor"};
  body = fopen("../shared/body.txt", "wb");
  fwrite(&error, sizeof(Error), 1, body);
  return INTERNAL_ERROR;
}
