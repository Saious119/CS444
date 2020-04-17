#include "queue.h"

/******* Unix (Linux/MacOS) *******/
#ifdef UNIX
	#include <pthread.h>
	#include <unistd.h>

	#define thread_t pthread_t
	#define sleep_milliseconds(ms) usleep(ms * 1000)

#endif

/******* Windows *******/
#ifdef WINDOWS
	#include <windows.h>

	#define thread_t HANDLE
	#define sleep_milliseconds(ms) Sleep(ms)

#endif

void create_thread(thread_t *t, void (*f)(queue *), queue *q);
void wait_for_threads(thread_t *ts, int c);
