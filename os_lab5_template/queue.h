#ifndef QUEUE_H
#define QUEUE_H

#define QUEUESIZE 5

#ifdef UNIX
	#include <pthread.h>
	#include <unistd.h>
  /*
   * TODO for students: Define your lock type here
   */
#endif
#ifdef WINDOWS
	#include <windows.h>
  /*
   * TODO for students: Define your lock type here
   */
#endif

typedef struct 
{
	int buff[QUEUESIZE];
	int index;
	int len;
  /*
   * TODO for students: Implement your lock here
   */
} queue;

queue *queueInit();
void queueDestroy(queue *q);
void queueLock(queue *q);
void queueUnlock(queue *q);

static inline int queueLen(queue *q) {return q->len;}
static inline int queueIsFull(queue *q) {return (q->len == QUEUESIZE);}
static inline int queueIsEmpty(queue *q) {return (q->len == 0);}

// Add item to the queue
static inline void enqueue(queue *q, int in)
{
  q->buff[(q->index + q->len++) % QUEUESIZE] = in;
}

// Remove item from queue
static inline int dequeue(queue *q)
{
  int n = q->buff[q->index];
  if ((q->index = (q->index + 1) % QUEUESIZE) == -1)
    q->index = QUEUESIZE - 1;
  --q->len;
  return n;
}

#endif
