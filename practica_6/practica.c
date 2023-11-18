#include <string.h>
#include <ncurses.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define MAX_LENGTH 50

#define KEY_ENCRYPT 10

typedef struct user
{
  char name[100];
  char email[100];
  char password[50];
} User;

typedef struct credentials
{
  char email[100];
  char password[50];
} Credentials;

void encryptPassword(char password[], int key)
{
  unsigned int i;
  for (i = 0; i < strlen(password); ++i)
  {
    password[i] = password[i] - key;
  }
}

int comparePasswords(char passwordInput[], char passwordSaved[], int key)
{
  unsigned int i;
  for (i = 0; i < strlen(passwordInput); ++i)
  {
    passwordInput[i] = passwordInput[i] + key;
  }
  return strcmp(passwordInput, passwordSaved) == 0;
}

int registerUserController(User userToCreate)
{

  FILE *usersFile = fopen("users.txt", "rb");
  int userAlreadyExists;
  User readUser;
  if (usersFile == NULL)
  {
    return 0;
  }

  while (fread(&readUser, sizeof(User), 1, usersFile) == 1)
  {
    if (strcmp(readUser.email, userToCreate.email) == 0)
    {
      return -1;
    }
  }
  fclose(usersFile);

  usersFile = fopen("users.txt", "ab");
  if (usersFile == NULL)
  {
    return 0;
  }
  char hash[SHA256_DIGEST_LENGTH * 2 + 1];
  encryptPassword(userToCreate.password, KEY_ENCRYPT);

  int opSuccessful = fwrite(&userToCreate, sizeof(User), 1, usersFile);
  fclose(usersFile);
  return 1;
}

int loginUserController(Credentials credentials)
{
  FILE *usersFile = fopen("users.txt", "rb");
  User readUser;
  if (usersFile == NULL)
  {
    return 0;
  }

  while (fread(&readUser, sizeof(User), 1, usersFile) == 1)
  {
    refresh();
    if (strcmp(readUser.email, credentials.email) == 0 && comparePasswords(readUser.password, credentials.password, KEY_ENCRYPT) == 1)
    {
      return 1;
    }
  }
  fclose(usersFile);

  return 0;
}

void displayMenu(WINDOW *menu_win, char *choices[], int highlight)
{
  int x, y, i;

  x = 2;
  y = 2;

  box(menu_win, 0, 0);

  for (i = 0; i < 3; ++i)
  {
    if (highlight == i + 1)
    {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, A_REVERSE);
    }
    else
    {
      mvwprintw(menu_win, y, x, "%s", choices[i]);
    }
    ++y;
  }

  wrefresh(menu_win);
}

int main()
{
  initscr();
  cbreak();
  keypad(stdscr, TRUE);

  int highlight = 1;
  int choice = 0;
  int c;

  char *menu_choices[] = {
      "Registrarse",
      "Iniciar sesion",
      "Exit"};

  int n_choices = 3;
  WINDOW *menu_win;

  menu_win = newwin(10, 30, 4, 4);
  keypad(menu_win, TRUE);

  mvprintw(0, 0, "Use arrow keys to go up and down, press Enter to select a choice");
  refresh();
  displayMenu(menu_win, menu_choices, highlight);

  while (1)
  {
    c = wgetch(menu_win);
    switch (c)
    {
    case KEY_UP:
      if (highlight == 1)
        highlight = n_choices;
      else
        --highlight;
      break;
    case KEY_DOWN:
      if (highlight == n_choices)
        highlight = 1;
      else
        ++highlight;
      break;
    case 10:
      choice = highlight;
      break;
    default:
      break;
    }
    displayMenu(menu_win, menu_choices, highlight);

    if (choice != 0)
    {
      clear();
      if (choice == 1)
      {
        User user;
        printw("Nombre: ");
        refresh();
        getstr(user.name);
        printw("Email: ");
        refresh();
        getstr(user.email);
        printw("Contraseña: ");
        refresh();
        getstr(user.password);
        int userCreated = registerUserController(user);
        refresh();
        clear();
        if (userCreated < 1)
        {
          mvprintw(0, 0, "Ocurrio un error al crear el usuario");
          getch();
        } else {
          mvprintw(0, 0, "Usuario registrado");
          getch();
        }

        choice = 0;
      }

      if (choice == 2)
      {
        Credentials login;
        printw("Email: ");
        refresh();
        getstr(login.email);
        printw("Contraseña: ");
        refresh();
        getstr(login.password);
        int userLogin = loginUserController(login);
        refresh();
        clear();
        if (userLogin == 0)
        {
          mvprintw(0, 0, "Ocurrio un error al iniciar sesion");
          getch();
        } else {
          mvprintw(0, 0, "Inicio de sesion exitoso");
          getch();
        }

        choice = 0;
      }

      if (choice == n_choices)
        break;
    }

    clear();
    refresh();
    displayMenu(menu_win, menu_choices, highlight);
  }

  endwin();
  return 0;
}
