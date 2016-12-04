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

const char HTML_STR[MAX_REQUEST_LENGTH] = "text/html";
const char TXT_STR[MAX_REQUEST_LENGTH] = "text/plain";
const char GIF_STR[MAX_REQUEST_LENGTH] = "image/gif";
const char JPEG_STR[MAX_REQUEST_LENGTH] = "image/jpeg";

const char ERROR_FILE[MAX_REQUEST_LENGTH] = "File not found.";
const char ERROR_EXT[MAX_REQUEST_LENGTH] = "File extension not allowed.";
const char ERROR_FBD[MAX_REQUEST_LENGTH] = "Permission Denied.";
const char ERROR_UNK[MAX_REQUEST_LENGTH] = "Unknown error.";

static pthread_mutex_t buffer_access = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t buffer_full = PTHREAD_COND_INITIALIZER;
static pthread_cond_t buffer_empty = PTHREAD_COND_INITIALIZER;

int dispatch_completed;

int port_num;
char path_prefix[1024];
int num_dispatcher;
int num_workers;
int queue_length;
int size_cache = 0;

//Structure for queue.
typedef struct request_queue
{
  int m_socket;
  char m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

request_queue_t* stack_req[MAX_QUEUE_SIZE];
int stack_top = 0;
int num_req = 0;

void * dispatch(void * arg)
{
  int fd;
  char filename[1024];
  request_queue_t* req_packet;
  while (1) {
    if ((fd = accept_connection()) < 0) {
      pthread_exit(NULL);
    }
    pthread_mutex_lock(&buffer_access);
    if (get_request(fd, filename) != 0) {
      continue;
    }
    while (stack_top == queue_length) {
      pthread_cond_wait(&buffer_empty, &buffer_access);
    }

    req_packet = (request_queue_t*) malloc(sizeof(request_queue_t));
    req_packet->m_socket = fd;
    strncpy(req_packet->m_szRequest, filename, MAX_REQUEST_LENGTH);
    stack_req[stack_top] = req_packet;
    stack_top++;

    pthread_cond_signal(&buffer_full);
    pthread_mutex_unlock(&buffer_access);
  }
  return NULL;
}

void * worker(void * arg)
{
  request_queue_t* req_packet;
  while (1) {
    pthread_mutex_lock(&buffer_access);
    while (stack_top == 0) {
      if (dispatch_completed == 1)
        pthread_exit(NULL);
      pthread_cond_wait(&buffer_full, &buffer_access);
    }

    stack_top--;
    num_req++;
    req_packet = stack_req[stack_top];

    pthread_cond_signal(&buffer_empty);
    pthread_mutex_unlock(&buffer_access);
  }
  return NULL;
}

int main(int argc, char **argv)
{
  //Error check first.
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
    printf("Please enter a valid num_dispatcher (1-%d)\n", MAX_THREADS);
    return -1;
  }
  num_workers = atoi(argv[4]);
  if (num_workers <= 0 || num_workers > MAX_THREADS) {
    printf("Please enter a valid num_workers (1-%d)\n", MAX_THREADS);
    return -1;
  }
  queue_length = atoi(argv[5]);
  if (queue_length > MAX_QUEUE_SIZE) {
    printf("Please enter a valid queue_length (1-%d)\n", MAX_QUEUE_SIZE);
    return -1;
  }
  if (argc == 7) {
    size_cache = atoi(argv[6]);
  } else {
    size_cache = 0;
  }

  dispatch_completed = 0;

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
  dispatch_completed = 1;
  for (i = 0; i < num_workers; i++) {
    if (pthread_join(t_workers[i], NULL) != 0)
      printf("Error joining worker thread\n");
  }
  return 0;
}
