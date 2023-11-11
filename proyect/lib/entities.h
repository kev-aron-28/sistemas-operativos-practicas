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
  long float actualBid;
  uuid_t lastBidderId;
  char id[37];
  char ownerId[37];
  char seller[200];
} Auction;

char * getName(User *);
char * getPassword(User*);
char * getEmail(User *);
void setName(User *, char name[100]);
void setPassword(User *, char password[20]);
void setEmail(User *, char email[100]);

#endif
