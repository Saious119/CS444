#include "thread.h"

void create_thread(thread_t *t, void (*f)(queue *), queue *q)
{
	#ifdef UNIX
		pthread_create((pthread_t *) t, NULL, (void *(*)(void *)) f, q);
	#endif

	#ifdef WINDOWS
		*t = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) f, q, 0, NULL);
	#endif
}

void wait_for_threads(thread_t *ts, int c)
{
	#ifdef UNIX
		if (c == 1)
		{
			pthread_join(*ts, NULL);
		}
		else
		{
			for (int i = 0; i < c; ++i)
			{
				pthread_join(ts[i], NULL);
			}
		}
	#endif

	#ifdef WINDOWS
		if (c == 1)
		{
			WaitForSingleObject(*ts, INFINITE);
		}
		else
		{
			WaitForMultipleObjects(c, ts, true, INFINITE);
		}
	#endif
}
