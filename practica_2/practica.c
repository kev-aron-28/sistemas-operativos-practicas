#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int createSharedMemory(size_t);
int createSemafore(int);
void down(int);
void up(int);
void fillMatrix(int (*)[9]);
void showMatrix(int (*)[9]);

int main(int argc, char const *argv[])
{
  pid_t process1, process2, process3;
  int statusProcess1, statusProcess2, statusProcess3;

  int sharedMemoryIdMatrix = createSharedMemory(sizeof(int[3][9]));
  int sharedMemoryResults = createSharedMemory(sizeof(int[3]));

  int semafore = createSemafore(1);

  int(*matrix)[9] = (int(*)[9])shmat(sharedMemoryIdMatrix, NULL, 0);
  int *results = (int *)shmat(sharedMemoryResults, NULL, 0);

  fillMatrix(matrix);
  showMatrix(matrix);

  if (matrix == (int(*)[9]) - 1)
  {
    shmdt(matrix);
    shmctl(sharedMemoryIdMatrix, IPC_RMID, NULL);
    perror("shmat");
    exit(1);
  }

  process1 = fork();
  if (process1 == 0)
  {
    int result = 0;
    printf("\n[1]Hola soy el proceso hijo 1\n");
    printf("\n\t[1]Mi identificador es: %d\n", getpid());
    printf("\n\t[1]Mi proceso padre es: %d\n", getppid());

    down(semafore);
      printf("\n\t[1]SUMAR LO SIGUIENTE: ");
      for (int i = 0; i < 9; i++)
      {
        int current = matrix[0][i];
        printf("%d ", current);
        result += current;
      }
      printf("\n");
      results[0] = result;
      printf("\n\t[1]El resultado de la suma de la fila 1 es %d \n", result);
    up(semafore);
    exit(0);
  }
  waitpid(process1, &statusProcess1, 0);
  process2 = fork();

  if (process2 == 0)
  {
    int result = 0;
    printf("\n[2]Hola soy el proceso hijo 2\n");
    printf("\n\t[2]Mi identificador es: %d\n", getpid());
    printf("\n\t[2]Mi proceso padre es: %d\n", getppid());
    up(semafore);
      printf("\n\t[2]SUMAR LO SIGUIENTE: ");
      for (int i = 0; i < 9; i++)
      {
        int current = matrix[1][i];
        printf("%d ", current);
        result += current;
      }
      printf("\n");
      results[1] = result;
      printf("\n\t[2]El resultado de la suma de la fila 2 es %d \n", result);
    down(semafore);
    exit(0);
  }
  waitpid(process2, &statusProcess2, 0);
  process3 = fork();

  if (process3 == 0)
  {
    int result = 0;
    printf("\n[3]Hola soy el proceso hijo 3\n");
    printf("\n\t[3]Mi identificador es: %d\n", getpid());
    printf("\n\t[3]Mi proceso padre es: %d\n", getppid());
    up(semafore);
      printf("\n\t[3]SUMAR LO SIGUIENTE: ");
      for (int i = 0; i < 9; i++)
      {
        int current = matrix[2][i];
        printf("%d ", current);
        result += current;
      }
      printf("\n");
      results[2] = result;
      printf("\n\t[3]El resultado de la suma de la fila 3 es %d \n", result);
    down(semafore);
    exit(0);
  }
  waitpid(process3, &statusProcess3, 0);

  printf("\nSoy el proceso padre\n");
  printf("Los resultados de la suma de las filas es (%d, %d, %d)\n", results[0], results[1], results[2]);
  return 0;
}

int createSharedMemory(size_t size)
{
  key_t keyForMemory = ftok("./token", 'Z');
  int shmId = shmget(keyForMemory, size, IPC_CREAT | 0660);

  if (shmId < 0)
  {
    shmctl(shmId, IPC_RMID, NULL);
    perror("shmget");
    exit(1);
  }

  return shmId;
}

int createSemafore(int initialValue)
{
  key_t keyForSemafore = ftok("./token2", 'S');
  int semaforeId = semget(keyForSemafore, 1, IPC_CREAT | 0644);
  if(semaforeId == -1) {
    return -1;
  }

  semctl(semaforeId, 0, SETVAL, initialValue);
  return semaforeId;
}

void down(int semid)
{
  struct sembuf op_p[] = {0, -1, 0};
  semop(semid, op_p, 1);
}

void up(int semid)
{
  struct sembuf op_v[] = {0, +1, 0};
  semop(semid, op_v, 1);
}

void fillMatrix(int (*matrix)[9])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (i == 0)
      {
        matrix[i][j] = j + 1;
      }

      if (i == 1)
      {
        matrix[i][j] = (j * 2) + 1;
      }

      if (i == 2)
      {
        matrix[i][j] = (j * 2) + 2;
      }
    }
  }
}

void showMatrix(int (*matrix)[9])
{
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}


