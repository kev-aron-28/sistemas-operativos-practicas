#ifndef ENTITIES_H
#define ENTITIES_H
#define ID_LEN 37
#include <uuid/uuid.h> //sudo apt-get install uuid-dev

typedef struct user {
  char name[100];
  char password[20];
  char email[100];
  char id[ID_LEN];
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
  char id[ID_LEN];
  char ownerId[ID_LEN];
  char sellerName[200];
} Auction;

typedef struct bid {
  char bidderId[ID_LEN];
  char auctionId[ID_LEN];
  float bid;
} Bid;

typedef struct getAuctionByOwnerId {
  char ownerId[ID_LEN];
} GetAuctionsByOwnerId;

typedef struct endAuctionById {
  char auctionId[ID_LEN];
  char userId[ID_LEN ];
} EndAuctionById;
#endif
