/***** Global Includes *****/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***** Local Includes *****/
#include "queue.h"
#include "thread.h"

/***** Constants *****/
#define TIME_TO_PRODUCE 1000 // milliseconds
#define TIME_TO_CONSUME 1000 // milliseconds
#define LOOP 10


/***** Producer *****
 * The function called by the producer thread.
 *
 * TODO for students: This function does not lock the queue and will
 *   ultimately race with the consumers. Experiment with locking and unlocking
 *   the producer using the queueLock() and queueUnlock() functions.
 */
void producer(queue *q)
{
  for (int i = 0; i < LOOP; ++i) 
  {
		// Pretend that expensive operations are occuring by sleeping.
    sleep_milliseconds(TIME_TO_PRODUCE); 
		
		// Busy loop while the queue is full. (HINT THIS IS AN UNSYNCHRONIZED READ)
		while (queueIsFull(q));
    
		// Put the produced item onto the queue and print that the enqueue occured.
		enqueue(q, i+1);
    printf("producer: produced %d th.\n",i+1);
  }
}

/***** Consumer *****
 * The function called by the consumer threads.
 *
 * TODO for students: The consumers does not lock the queue when consuming
 *   and ultimately race with the producer and other consumers. Experiment
 *   with the queueLock() and queueUnlock() functions within this function.
 */
void consumer(queue *q)
{
  // Simulate the consumer's random quick arrival (Makes race conditions prominent)
  sleep_milliseconds((rand()%LOOP) * TIME_TO_CONSUME/LOOP); 

	// Wait for an item to be in the queue. (HINT THIS IS AN UNSYNCHRONIZED READ)
  while (queueIsEmpty(q)); 

	// Pretend to consume
  sleep_milliseconds(TIME_TO_CONSUME); 

  printf("------------------------------------>consumer: recieved %d.\n", dequeue(q));		
}

int main()
{
	// Initialize the queue. If the queue fails to initialize, kill the program.
  queue *q = queueInit();
  if (q == NULL) 
  {
    fprintf(stderr, "main: Queue Init failed.\n");
    exit(1);
  }

  // Seed the random number generator using the system clock. 
  unsigned int iseed = (unsigned int) time(NULL);
  srand(iseed);

  // Start one producer thread and as many consumer threads as specified above.
  thread_t pro, con[LOOP];

	create_thread(&pro, producer, q);
	for (int i = 0; i < LOOP; ++i)
  {
		sleep_milliseconds((rand() % 2) * 1000);
    create_thread(&con[i], consumer, q);
  }
	
	// Let the producers and consumers finish before cleaning up.
	wait_for_threads(&pro, 1);
	wait_for_threads(con, LOOP);

	// Clean up the queue.
  queueDestroy(q);

#ifdef WINDOWS
	// If this is running in Windows, execute the "pause" shell command so that
	// the command prompt window doesn't close after this program finishes.
  system("pause");
#endif

  return 0;
}
