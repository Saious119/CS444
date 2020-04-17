#include <stdlib.h>
#include "queue.h"

queue *queueInit()
{
  queue *q;

  if ((q = (queue *) malloc(sizeof(queue))) == NULL)
    return NULL;

  /*
   * TODO for students: Remember to allocate your lock here if it needs
   * to be dynamically allocated
   */

  q->len = q->index = 0;

#ifdef UNIX
  /*
   * TODO for students: Initialize the lock here
   */
#endif

#ifdef WINDOWS
  /*
   * TODO for students: Initialize the lock here
   */
#endif

  return q;
}

void queueDestroy(queue *q)
{
  /*
   * TODO for students: free the locks here
   */


  // Free memory used for the queue
  free(q);
}

void queueLock(queue *q)
{
#ifdef UNIX
  /*
   * TODO for students: Implement locking for UNIX systems.
   */
#endif

#ifdef WINDOWS
  /*
   * TODO for students: Implement locking for Windows systems.
   */
#endif
}

void queueUnlock(queue *q)
{
#ifdef UNIX
  /*
   * TODO for students: Implement unlocking for UNIX systems.
   */
#endif

#ifdef WINDOWS
  /*
   * TODO for students: Implement unlocking for Windows systems.
   */
#endif
}
