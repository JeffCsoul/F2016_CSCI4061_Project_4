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

//Structure for queue.
typedef struct request_queue
{
  int m_socket;
  char m_szRequest[MAX_REQUEST_LENGTH];
} request_queue_t;

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
  int size_cache;

  if(argc != 6 && argc != 7)
  {
    printf("usage: %s port path num_dispatcher num_workers queue_length [cache_size]\n", argv[0]);
    return -1;
  }

  port_num = atoi(argv[1]);
  strncpy(path_prefix, argv[2], 1024);
  num_dispatcher = atoi(argv[3]);
  num_workers = atoi(argv[4]);
  queue_length = atoi(argv[5]);
  if (argc == 7) {
    size_cache = atoi(argv[6]);
  }

  printf("Call init() first and make a dispather and worker threads\n");
  init(port_num);
  return 0;
}
