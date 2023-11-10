#include <stdio.h>
#include <string.h>
#include "entities.h"

char * getName(User * user) {
  return user->name;
}

char * getPassword(User* user) {
  return user->password;
}

char * getEmail(User * user) {
  return user->email;
}

void setName(User * user, char name[100]) {
  strcpy(user->name, name);
}

void setPassword(User * user, char password[20]) {
  strcpy(user->password, password);
}

void setEmail(User * user, char email[100]) {
  strcpy(user->email, email);
}
