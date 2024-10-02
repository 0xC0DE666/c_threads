#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "./c_structs.h"

void ptr_free(void** p) {
  free(*p);
  *p = NULL;
}

int x = 0;
int nums[] = {0};
void* add(void* arg) {
    pthread_t self = pthread_self();
    Array* arr = (Array*) arg;
    nums[x] = x;
    int res = array_append(arr, &nums[x]);
    x++;
    if (res == 0) {
      printf("thread %lu added %d\n", self, x);
    } else {
      printf("thread %lu failed to add %d\n", self, x);
    }
    pthread_exit(NULL);  // Terminates the thread
}


int random_int(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void* print_self(void* arg) {
  pthread_t self = pthread_self();
  int  r = random_int(1, 10);
  printf("Thread %lu running %d\n", self, r);
  sleep(r);
  printf("Thread %lu done\n", self);
  pthread_exit(NULL);
}

char* n_to_str(int* n) {
  char* str = malloc(10 * sizeof(char));
  sprintf(str, "%d", *n);
  return str;
}


int main() {
  // Seed the random number generator with the current time
  srand(time(NULL));

  Array* numbers = array_new(10);
  Array* threads = array_new(20);
  
  for (int i = 0; i < threads->capacity; ++i) {
    array_append(threads, malloc(sizeof(pthread_t)));
  }
  
  for (int i = 0; i < 10; ++i) {
    pthread_t* t = (pthread_t*) array_get(threads, i);
    int res_a = pthread_create(t, NULL, print_self, NULL);
  }
  
  for (int i = 0; i < 10; ++i) {
    pthread_t* t = (pthread_t*) array_get(threads, i);
    pthread_join(*t, NULL);
  }
  printf("All threads done.\n");

  array_free(&numbers, NULL);
  array_free(&threads, (FreeFn) ptr_free);
  
  return 0;
}
