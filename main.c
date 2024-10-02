#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "./c_structs.h"

void ptr_free(void** p) {
  free(*p);
  *p = NULL;
}

int random_int(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

typedef struct Job {
  int id;
  int duration;
  char* name;
} Job;

Job* job_new(int id, int duration) {
  Job* j = malloc(sizeof(Job));

  j->id = id;
  j->duration = duration;

  return j;
}

void* print_self(void* arg) {
  pthread_t self = pthread_self();
  int  r = random_int(1, 10);
  printf("Thread %lu running %d\n", self, r);
  sleep(r);
  printf("Thread %lu done\n", self);
  pthread_exit(NULL);
}

void* exec_job(void* job) {
    pthread_t self = pthread_self();
    Job* j = (Job*) job;
    printf("thread %lu starting job %d\n", self, j->id);
    sleep(j->duration);
    printf("thread %lu finished job %d in %d seconds\n", self, j->id, j->duration);
    pthread_exit(NULL);  // Terminates the thread
}

int main() {
  srand(time(NULL));

  Array* threads = array_new(4);
  Array* jobs = array_new(10);
  
  for (int i = 0; i < threads->capacity; ++i) {
    array_append(threads, malloc(sizeof(pthread_t)));
  }

  for (int i = 0; i < jobs->capacity; ++i) {
    Job* j = job_new(i, random_int(1, 10));
    array_append(jobs, j);
  }
  
  int busy_threads = 0;
  while(jobs->size > 0) {
    pthread_t* t = (pthread_t*) array_get(threads, busy_threads);
    Job* j = array_remove(jobs, 0);

    int fail = pthread_create(t, NULL, exec_job, (void*)j);

    if (fail) {
      printf("failed to exec job %d\n", j->id);
    } else {
      ++busy_threads;
    }

    bool threads_busy = busy_threads > 0;
    bool all_threads_busy = busy_threads == threads->size;
    bool jobs_empty = jobs->size == 0;

    if (all_threads_busy || (jobs_empty && threads_busy)) {
      int busy_count = all_threads_busy ? threads->size : busy_threads;
      for (int i = 0; i < busy_count; ++i) {
        pthread_t* t = (pthread_t*) array_get(threads, i);
        pthread_join(*t, NULL);
      }
      busy_threads = 0;
    }
  }
  
  printf("Jobs done.\n");

  array_free(&jobs, (FreeFn) ptr_free);
  array_free(&threads, (FreeFn) ptr_free);
  
  return 0;
}
