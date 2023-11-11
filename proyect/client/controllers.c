#include <stdio.h>
#include "../lib/lib.h"
#include "controllers.h"
#include "../lib/entities.h"
#include "../lib/response.h"

User registerUserController()
{
  User user;
  printf("NOMBRE: ");
  scanf("%s", user.name);

  printf("EMAIL: ");
  scanf("%s", user.email);

  printf("CONTRASENA: ");
  scanf("%s", user.password);

  return user;
}

Login loginUserController() {
  Login login;

  printf("EMAIL: ");
  scanf("%s", login.email);

  printf("CONTRASENA: ");
  scanf("%s", login.password);

  printf("email: %s \n", login.email);

  return login;
}
