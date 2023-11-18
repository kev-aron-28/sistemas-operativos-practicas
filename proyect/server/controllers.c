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
    fclose(body);
    return INTERNAL_ERROR;
  }
  uuid_t userId;
  uuid_generate(userId);

  uuid_unparse(userId, userToCreate.id);
  int opSuccessful = 0;
  printf("USER ID %s", userToCreate.id);
  opSuccessful = fwrite(&userToCreate, sizeof(User), 1, usersFile);
  fclose(usersFile);

  if (opSuccessful)
  {
    printf("ESCRIBIENDO EN BODY");
    body = fopen("../shared/body.txt", "wb");
    fwrite(&userToCreate, sizeof(User), 1, body);
    fclose(body);
    return OK;
  }

  Error error = {"Error en el servidor"};
  body = fopen("../shared/body.txt", "wb");
  fwrite(&error, sizeof(Error), 1, body);
  fclose(body);
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

STATUS createAuctionController()
{
  FILE *body = fopen("../shared/body.txt", "rb");
  Auction auctionToCreate;
  int isBodyOk = fread(&auctionToCreate, sizeof(Auction), 1, body);
  
  if (isBodyOk == 0) {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  FILE *auctionsFile = fopen("../shared/auctions.txt", "ab");

  uuid_t auctionId;
  uuid_generate(auctionId);

  uuid_unparse(auctionId, auctionToCreate.id);
  
  int opSuccessful = 0;
  opSuccessful = fwrite(&auctionToCreate, sizeof(Auction), 1, auctionsFile);
  fclose(auctionsFile);

  if (opSuccessful)
  {
    body = fopen("../shared/body.txt", "wb");
    fwrite(&auctionToCreate, sizeof(Auction), 1, body);
    fclose(body);

    return OK;
  }

  Error error = {"Error en el servidor"};
  body = fopen("../shared/body.txt", "wb");
  fwrite(&error, sizeof(Error), 1, body);
  fclose(body);

  return INTERNAL_ERROR;
}

STATUS getAllAuctionsController()
{
  FILE *body = fopen("../shared/body.txt", "wb");
  FILE *auctionsFile = fopen("../shared/auctions.txt", "rb");

  body = fopen("../shared/body.txt", "ab");

  if (body == NULL || auctionsFile == NULL)
  {
    Error error = {"Error del servidor"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return INTERNAL_ERROR;
  }

  Auction readAuction;
  while (fread(&readAuction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    fwrite(&readAuction, sizeof(Auction), 1, body);
  }
  fclose(body);
  return OK;
}

STATUS bidToAuctionController() {
  FILE *body = fopen("../shared/body.txt", "rb");
  Bid bid;
  int isBodyOk = fread(&bid, sizeof(Bid), 1, body);

  if (isBodyOk == 0) {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  int indexOfRecord = 0;
  int auctionFound = 0;
  FILE *auctionsFile = fopen("../shared/auctions.txt", "rb+");
  Auction auction;
  while (fread(&auction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    if (strcmp(bid.auctionId, auction.id) == 0)
    {
      auctionFound = 1;
      break;
    }
    indexOfRecord++;
  }

  if(!auctionFound) {
    Error error = {"No se encontro el registro"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);

  if(bid.bid > auction.actualBid) {
    auction.actualBid = bid.bid;
    strcpy(auction.lastBidderId, bid.bidderId);
    fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);
    fwrite(&auction, sizeof(Auction), 1, auctionsFile);
  }
  fclose(auctionsFile);  
  return OK;
}
