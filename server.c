/* csci4061 F2016 Assignment 4
* section: 9
* date: 11/30/2016
* names: Tiannan Zhou, Annelies Odermann
* UMN Internet ID, Student ID zhou0745(5232494), oderm008(4740784)
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "util.h"

#define MAX_THREADS 100
#define MAX_QUEUE_SIZE 100
#define MAX_REQUEST_LENGTH 64

static pthread_mutex_t buffer_access = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;

//Structure for queue.
typedef struct request_queue
{
  int m_socket;
  char m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

request_queue_t* stack_req[MAX_QUEUE_SIZE];
int num_req = 0;

void * dispatch(void * arg)
{
  return NULL;
}

void * worker(void * arg)
{
  return NULL;
}

int main(int argc, char **argv)
{
  //Error check first.
  int port_num;
  char path_prefix[1024];
  int num_dispatcher;
  int num_workers;
  int queue_length;
  int size_cache = 0;
  pthread_t t_dispathers[MAX_THREADS];
  pthread_t t_workers[MAX_THREADS];

  if(argc != 6 && argc != 7)
  {
    printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
    return -1;
  }

  port_num = atoi(argv[1]);
  strncpy(path_prefix, argv[2], 1024);
  num_dispatcher = atoi(argv[3]);
  if (num_dispatcher <= 0 || num_dispatcher > MAX_THREADS) {
    printf("Please enter a valid num_dispatcher (1-100)\n");
    return -1;
  }
  num_workers = atoi(argv[4]);
  if (num_workers <= 0 || num_workers > MAX_THREADS) {
    printf("Please enter a valid num_workers (1-100)\n");
    return -1;
  }
  queue_length = atoi(argv[5]);
  if (argc == 7) {
    size_cache = atoi(argv[6]);
  } else {
    size_cache = 0;
  }

  printf("Call init() first and make dispather and worker threads\n");
  init(port_num);

  int i;
  for (i = 0; i < num_dispatcher; i++) {
    if (pthread_create(&t_dispathers[i], NULL, dispatch, NULL) != 0) {
      printf("Error creating dispatch thread\n");
      exit(1);
    }
  }
  for (i = 0; i < num_workers; i++) {
    if (pthread_create(&t_workers[i], NULL, worker, NULL) != 0) {
      printf("Error creating worker thread\n");
      exit(1);
    }
  }

  for (i = 0; i < num_dispatcher; i++) {
    if (pthread_join(t_dispathers[i], NULL) != 0)
      printf("Error joining dispatch thread\n");
  }
  for (i = 0; i < num_workers; i++) {
    if (pthread_join(t_workers[i], NULL) != 0)
      printf("Error joining worker thread\n");
  }
  return 0;
}
