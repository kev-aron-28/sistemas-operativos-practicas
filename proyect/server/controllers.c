#include "controllers.h"
#include <stdio.h>
#include "../lib/entities.h"
#include "../lib/response.h"
#include "../lib/lib.h"

STATUS registerUserController()
{

  User user = {"Kevin", "1234", "kevaron_28"};
  User user1;
  FILE *usersFile = fopen("../shared/users.txt", "ab");
  if (usersFile == NULL)
  {
    printf("Error al abrir el archivo.\n");
    return INTERNAL_ERROR;
  }

  int flag = 0;
  flag = fwrite(&user, sizeof(User), 1, usersFile);
  fclose(usersFile);

  usersFile = fopen("../shared/users.txt", "rb");
  User read;
  while (fread(&read, sizeof(User), 1, usersFile) == 1)
  {
    printf("Name: %s\n", read.name);
    printf("password: %s\n", read.password);
    printf("email %s\n", read.email);
  }

  if (flag)
  {
    return OK;
  }

  
  return INTERNAL_ERROR;
}
