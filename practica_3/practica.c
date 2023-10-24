#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void fillMatrix(int [3][9]);
void showMatrix(int [3][9]);

pthread_mutex_t mutex;
struct op_thread
{
  int arr[3][9];
  int results[3];
} op;

void *thread(void *args)
{

  int indexToSaveResult = (int*) args;
  int result = 1;
  pthread_t tid = pthread_self(); 

  printf("HILO CON ID: %ld\n", tid);
  printf("MULTIPLICAR LO SIGUIENTE [%d]:\n", indexToSaveResult);
  for (int i = 0; i < 9; i++)
  {
    int current = op.arr[indexToSaveResult][i];
    result *= current;
    if(i == 8) {
      printf("%d", current);
    } else {
      printf("%d*", current);
    }
  }
    printf("\n");

  pthread_mutex_lock(&mutex);
  op.results[indexToSaveResult] = result;
  pthread_mutex_unlock(&mutex);
  printf("\n");
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[3];
  fillMatrix(op.arr);
  showMatrix(op.arr);
  printf("\nCreacion del hilo...\n");

  pthread_mutex_init(&mutex, NULL);

  for (int numOfThreads = 0; numOfThreads < 3; numOfThreads++)
  {
    pthread_create(&threads[numOfThreads], NULL, thread, (void *)numOfThreads);
  }

   for (int i = 0; i < 3; i++) {
    pthread_join(threads[i], NULL);
  }
  pthread_mutex_destroy(&mutex);

  printf("\nHilos creados. Las multiplicaciones de cada fila son (%d, %d, %d)", op.results[0], op.results[1], op.results[2]);

}

void fillMatrix(int matrix[3][9])
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

void showMatrix(int matrix [3][9])
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
