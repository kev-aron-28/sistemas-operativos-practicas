#include "controllers.h"
#include <stdio.h>
#include "../lib/entities.h"
#include "../lib/response.h"
#include "../lib/lib.h"

STATUS registerUserController()
{

  FILE * body = fopen("../shared/body.txt", "rb");
  User user;
  fread(&user, sizeof(User), 1, body);
  fclose(body);

  FILE *usersFile = fopen("../shared/users.txt", "ab");
  if (usersFile == NULL)
  {
    printf("Error al abrir el archivo.\n");
    return INTERNAL_ERROR;
  }

  int flag = 0;
  flag = fwrite(&user, sizeof(User), 1, usersFile);
  fclose(usersFile);
  if (flag)
  {
    return OK;
  }

  return INTERNAL_ERROR;
}
