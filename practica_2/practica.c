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
  pid_t process;

  int sharedMemoryIdMatrix = createSharedMemory(sizeof(int[3][9]));
  int sharedMemoryIdResults = createSharedMemory(sizeof(int[3]));

  int semafore = createSemafore(1);

  int(*matrix)[9] = (int(*)[9])shmat(sharedMemoryIdMatrix, NULL, 0);
  int *results = (int *)shmat(sharedMemoryIdResults, NULL, 0);

  fillMatrix(matrix);
  showMatrix(matrix);

  if (matrix == (int(*)[9]) - 1)
  {
    shmdt(matrix);
    shmctl(sharedMemoryIdMatrix, IPC_RMID, NULL);
    perror("shmat");
    exit(1);
  }

  for (int i = 0; i < 3; i++)
  {
    process = fork();
    int result = 0;
    if(process == 0) {
      printf("\n[%d]Hola soy el proceso hijo %d\n", i + 1, i + 1);
      printf("\n\t[%d]Mi identificador es: %d\n", i + 1, getpid());
      printf("\n\t[%d]Mi proceso padre es: %d\n",  i + 1, getppid());
      up(semafore);
        printf("\n\t[%d]SUMAR LO SIGUIENTE: ", i + 1);
        for (int j = 0; j < 9; j++)
        {
          int current = matrix[i][j];
          printf("%d ", current);
          result += current;
        }
        printf("\n");
        results[i] = result;
        printf("\n\t[%d]El resultado de la suma de la fila %d es %d \n", i + 1 , i + 1, result);
      down(semafore);
      exit(0);
    }
    result = 0;
  }
  
  for(int i=0;i<3;i++)
  wait(NULL);
  

  printf("\nSoy el proceso padre\n");
  printf("Los resultados de la suma de las filas es (%d, %d, %d)\n", results[0], results[1], results[2]);

  shmdt(matrix);
  shmdt(results);
  shmctl(sharedMemoryIdMatrix, IPC_RMID, NULL);
  shmctl(sharedMemoryIdResults, IPC_RMID, NULL);
  semctl(semafore, 0, IPC_RMID);
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
  int semaforeId = semget(keyForSemafore, -1, IPC_CREAT | 0644);
  if (semaforeId == -1)
  {
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
