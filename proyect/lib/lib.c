#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib.h"

void encryptPassword(char password[])
{
  unsigned int i;
  for (i = 0; i < strlen(password); ++i)
  {
    password[i] = password[i] - KEY_ENCRYPT;
  }
}

int comparePasswords(char passwordInput[], char passwordSaved[])
{
  unsigned int i;
  for (i = 0; i < strlen(passwordInput); ++i)
  {
    passwordInput[i] = passwordInput[i] + KEY_ENCRYPT;
  }
  return strcmp(passwordInput, passwordSaved) == 0;
}
