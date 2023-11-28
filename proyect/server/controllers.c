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
  encryptPassword(userToCreate.password);
  printf("PASSWORD ENCRYPTED %s", userToCreate.password);

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
    if (strcmp(readUser.email, login.email) == 0 && comparePasswords(readUser.password, login.password) == 1)
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

  if (isBodyOk == 0)
  {
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

  int countAuctions = 0;
  Auction readAuction;
  while (fread(&readAuction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    if (readAuction.isActive == 1)
    {
      fwrite(&readAuction, sizeof(Auction), 1, body);
      countAuctions++;
    }
  }

  if(countAuctions == 0) {
    Error error = {"No se encontraron subastas"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return NOT_FOUND;
  }

  fclose(auctionsFile);
  fclose(body);
  return OK;
}

STATUS bidToAuctionController()
{
  FILE *body = fopen("../shared/body.txt", "rb");
  Bid bid;
  int isBodyOk = fread(&bid, sizeof(Bid), 1, body);

  if (isBodyOk == 0)
  {
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
    if (strcmp(bid.auctionId, auction.id) == 0 && auction.isActive == 1)
    {
      auctionFound = 1;
      break;
    }
    indexOfRecord++;
  }

  if (!auctionFound)
  {
    Error error = {"No se encontro el registro o ya no existe"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);

  if (bid.bid > auction.actualBid)
  {
    auction.actualBid = bid.bid;
    strcpy(auction.lastBidderId, bid.bidderId);
    fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);
    fwrite(&auction, sizeof(Auction), 1, auctionsFile);
  }
  fclose(auctionsFile);
  fclose(body);
  return OK;
}

STATUS endAuctionController()
{
  FILE *body = fopen("../shared/body.txt", "rb");
  FILE *auctionsFile = fopen("../shared/auctions.txt", "rb+");
  EndAuctionById auctionToEnd;
  int isBodyOk = fread(&auctionToEnd, sizeof(EndAuctionById), 1, body);

  if (isBodyOk == 0)
  {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  int indexOfRecord = 0;
  int auctionFound = 0;
  Auction auction;
  while (fread(&auction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    if (
        strcmp(auction.ownerId, auctionToEnd.userId) == 0 &&
        strcmp(auction.id, auctionToEnd.auctionId) == 0)
    {
      auctionFound = 1;
      break;
    }
    indexOfRecord++;
  }

  if (!auctionFound)
  {
    Error error = {"No se encontro el registro"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }
  
  fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);
  auction.isActive = 0;
  fseek(auctionsFile, indexOfRecord * sizeof(Auction), SEEK_SET);
  fwrite(&auction, sizeof(Auction), 1, auctionsFile);

  body = fopen("../shared/body.txt", "wb");
  BodyMessage message = {"Se a terminado la subasta"};
  fwrite(&message, sizeof(BodyMessage), 1, body);
  fclose(body);
  fclose(auctionsFile);
  return OK;
}

STATUS getAuctionsByOwnerId()
{
  FILE *body = fopen("../shared/body.txt", "rb");
  FILE *auctionsFile = fopen("../shared/auctions.txt", "rb");
  GetAuctionsByOwnerId owner;
  int isBodyOk = fread(&owner, sizeof(GetAuctionsByOwnerId), 1, body);

  if (isBodyOk == 0)
  {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  body = fopen("../shared/body.txt", "wb");
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
  int count = 0;
  while (fread(&readAuction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    if (strcmp(readAuction.ownerId, owner.ownerId) == 0)
    {
      count++;
      fwrite(&readAuction, sizeof(Auction), 1, body);
    }
  }

  if (count == 0)
  {
    Error error = {"No haz creado subastas"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return INTERNAL_ERROR;
  }
  fclose(auctionsFile);
  fclose(body);
  return OK;
}

STATUS getAuctionsWonByUserId (){
  FILE *body = fopen("../shared/body.txt", "rb");
  FILE *auctionsFile = fopen("../shared/auctions.txt", "rb");
  GetAuctionsByOwnerId userToFilterAuctions;
  int isBodyOk = fread(&userToFilterAuctions, sizeof(GetAuctionsByOwnerId), 1, body);

  if (isBodyOk == 0)
  {
    Error error = {"Faltan propiedades en el body"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return BAD_REQUEST;
  }

  body = fopen("../shared/body.txt", "wb");
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
  int count = 0;
  while (fread(&readAuction, sizeof(Auction), 1, auctionsFile) == 1)
  {
    if (strcmp(readAuction.lastBidderId, userToFilterAuctions.ownerId) == 0 && !readAuction.isActive)
    {
      count++;
      fwrite(&readAuction, sizeof(Auction), 1, body);
    }
  }

  if (count == 0)
  {
    Error error = {"No haz ganado ninguna subasta"};
    body = fopen("../shared/body.txt", "wb");
    fwrite(&error, sizeof(Error), 1, body);
    fclose(body);
    return INTERNAL_ERROR;
  }

  fclose(auctionsFile);
  fclose(body);
  return OK;
}
