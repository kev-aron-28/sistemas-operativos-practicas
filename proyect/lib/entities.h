#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct user {
  char name[100];
  char password[20];
  char email[100];
} User;

char * getName(User *);
char * getPassword(User*);
char * getEmail(User *);
void setName(User *, char name[100]);
void setPassword(User *, char password[20]);
void setEmail(User *, char email[100]);

#endif
