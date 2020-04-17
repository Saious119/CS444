/***** Includes *****/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Linux or MacOS
#ifdef UNIX
	#include <pthread.h>
	#include <unistd.h>
#endif

//Windows
#ifdef WINDOWS
	#include <windows.h>
#endif

/***** Defines: Constants in seconds *****/
#define PRODUCER_SLEEP_S	1
#define CONSUMER_SLEEP_S	1
#define QUEUESIZE 			5
#define LOOP 				10


/***** Function prototypes *****/
void *producer (void *args);
void *consumer (void *args);

/***** Queue struct *****/
typedef struct 
{
	int buf[QUEUESIZE];
	long head, tail;
	int full, empty;  //you may need or may not
	/*TODO for students: Declare the locks here */
	#ifdef UNIX
		pthread_mutex_t m;
	#endif
	#ifdef WINDOWS
		HANDLE winM;
	#endif
} queue;

/***** Queue function prototypes  *****/
queue *queueInit (void);
void queueDelete (queue *q);
void queueAdd (queue *q, int in);
void queueDel (queue *q, int *out);

/***** main *****/
int main ()
{
	queue *fifo;
	int i;

	//for Consumer's random coming
	unsigned int iseed = (unsigned int)time(NULL);
	//seeds the random number generator
	srand(iseed);
	
    /******one producer and multiple consumer********/
	
	//an array of consumers and one producer
	#ifdef UNIX
	pthread_t pro, con[LOOP];
	#endif
	
	//an array of consumers and one producer
	#ifdef WINDOWS
	HANDLE pro, con[LOOP];
	#endif
	
	fifo = queueInit ();
	if (fifo ==  NULL) 
	{
		fprintf (stderr, "main: Queue Init failed.\n");
		exit (1);
	}
	
	#ifdef UNIX
		pthread_create (&pro, NULL, producer, fifo);
		for(i=0; i<LOOP; i++)
		{
			pthread_create (&con[i], NULL, consumer, fifo);
		}
	#endif
	
	#ifdef WINDOWS
		pro = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) producer, fifo, 0, NULL);
		for(i=0; i<LOOP; i++)
		{
	#ifdef WINDOWS
			/* TODO for students: Simulate Consumers' random arrival */
			Sleep((rand()%2)*1000);
	#endif
		    con[i] = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE) consumer, fifo, 0, NULL);		
		}
	#endif
	
	
	#ifdef UNIX
		pthread_join (pro, NULL);
		for(i=0; i<LOOP; i++)
		{
			pthread_join (con[i], NULL);	
		}
	#endif
	
	#ifdef WINDOWS
		WaitForSingleObject(pro, INFINITE);
        /*******Wait for all the threads complete**********/      
		WaitForMultipleObjects(LOOP, con, TRUE, INFINITE);	

	#endif
	
	queueDelete (fifo);

    #ifdef WINDOWS
	system("pause");
	#endif
	
	return 0;
}

/***** producer *****/
void *producer (void *q)
{

	queue *fifo;
	int i;

	fifo = (queue *)q;

    //TODO for students: obtain the lock and release the lock somewhere
	for (i = 0; i < LOOP; i++) 
	{

        // TODO for students: Obtain the locks somewhere here
		#ifdef UNIX
		pthread_mutex_lock(&fifo->m);
		while (fifo->full) 
		{
			pthread_mutex_unlock(&fifo->m);
			pthread_mutex_lock(&fifo->m);
		}
		#endif
		
        #ifdef WINDOWS
		WaitForSingleObject(fifo->winM, INFINITE);
		while (fifo->full) {
			ReleaseMutex(fifo->winM);
			WaitForSingleObject(fifo->winM, INFINITE);
		}

        #endif
			
		
		queueAdd (fifo, i+1);
		printf ("producer: produced %d th.\n",i+1);
		#ifdef UNIX
		pthread_mutex_unlock(&fifo->m);
		#endif 

		#ifdef WINDOWS
		ReleaseMutex(fifo->winM);
		#endif
		
		/* sleep */
		#ifdef UNIX
			usleep ( PRODUCER_SLEEP_S * 1000000); 
		#endif
		
		#ifdef WINDOWS
			Sleep ( PRODUCER_SLEEP_S * 1000);	
		#endif

        /*******Release the locks**********/
        #ifdef UNIX
			pthread_mutex_unlock(&fifo->m);
        #endif

        
        #ifdef WINDOWS
			ReleaseMutex(fifo->winM);
        #endif
	}

	return (NULL);
}

/***** consumer *****/
void *consumer (void *q)
{
	queue *fifo;
	int d;

	fifo = (queue *)q;

	/* Simulate Consumers' random arrival */
	#ifdef UNIX
		usleep ( (rand()%LOOP) * 1000000); 
	#endif
		

    // TODO for students: obtain the lock and release the lock somewhere

    #ifdef UNIX
	pthread_mutex_lock(&fifo->m);
	while (fifo->empty) 
	{
		pthread_mutex_unlock(&fifo->m);
		pthread_mutex_lock(&fifo->m);
	}
	#endif
	
	
    #ifdef WINDOWS
	WaitForSingleObject(fifo->winM, INFINITE);
	while (fifo->empty)
	{
		ReleaseMutex(fifo->winM);
		WaitForSingleObject(fifo->winM, INFINITE);
	}
    #endif
	
	/* sleep */
	#ifdef UNIX
		usleep ( CONSUMER_SLEEP_S * 1000000); 
	#endif

	#ifdef WINDOWS
		Sleep ( CONSUMER_SLEEP_S * 1000);	
	#endif

    
	queueDel (fifo, &d);
	printf ("------------------------------------>consumer: recieved %d.\n", d);		

    #ifdef UNIX
		pthread_mutex_unlock(&fifo->m);
    #endif


    #ifdef WINDOWS
		ReleaseMutex(fifo->winM);
    #endif
	

	return (NULL);
}


/***** queueInit *****/
queue *queueInit (void)
{
	queue *q;
	int i;
	
	q = (queue *)malloc (sizeof (queue));
	if (q == NULL) return (NULL);

	for(i=0;i<QUEUESIZE;i++)
	{
		q->buf[i]=0;
	}
	q->empty = 1;
	q->full = 0;
	q->head = 0;
	q->tail = 0;

    //TODO for students: Initialize the locks here
	#ifdef UNIX
		pthread_mutex_init(&q->m, NULL);
	#endif
	#ifdef WINDOWS
		q->winM = CreateMutex(NULL, FALSE, NULL);
	#endif
	

	return (q);
}


/***** queueDelete *****/
void queueDelete (queue *q)
{

	//TODO for students: free the locks here
	#ifdef UNIX
		pthread_mutex_unlock(&q->m);
	#endif
	#ifdef WINDOWS
		ReleaseMutex(&q->winM);
	#endif

	/* free memory used for queue */
	free (q);
}

/***** queueAdd *****/
void queueAdd (queue *q, int in)
{
	q->buf[q->tail] = in;
	q->tail++;
	if (q->tail == QUEUESIZE)
		q->tail = 0;
	if (q->tail == q->head)
		q->full = 1;
	q->empty = 0;

	return;
}

/***** queueDel *****/
void queueDel (queue *q, int *out)
{
	*out = q->buf[q->head];
	q->buf[q->head]=0;

	q->head++;

	if (q->head == QUEUESIZE)
		q->head = 0;
	if (q->head == q->tail)
		q->empty = 1;
	q->full = 0;

	return;
}
