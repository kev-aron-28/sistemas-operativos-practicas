#ifndef ENTITIES_H
#define ENTITIES_H
#include <uuid/uuid.h> //sudo apt-get install uuid-dev

typedef struct user {
  char name[100];
  char password[20];
  char email[100];
  char id[37];
} User;

typedef struct error {
  char message[256];
} Error;

typedef struct login {
  char email[100];
  char password[20];
} Login;

typedef struct bodyMessage {
  char message[256];
} BodyMessage;

typedef struct token {
  char token[256];
} Token;

typedef struct auction {
  char title[100];
  int isActive;
  float actualBid;
  char lastBidderId[100];
  char id[37];
  char ownerId[37];
  char sellerName[200];
} Auction;

typedef struct bid {
  char bidderId[37];
  char auctionId[37];
  float bid;
} Bid;

#endif
