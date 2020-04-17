//Written by Graham Northup edited by Marzieh Babaeianjelodar

#include <stdio.h>
#include <stdlib.h>

//if you want to compile the thread part or the fork part.
#if !defined(FORK) && !defined(THREAD)
#error Please compile either with -DFORK (for the fork based implementation) or -DTHREAD (for the pthread based implementation)
#endif

#ifdef THREAD
#include <pthread.h>
#else  /* #ifdef FORK */
#include <unistd.h>
#include <sys/wait.h>
#endif


int shared = 3; //global variable

#define THREAD_NAME "my_thread: " //constant

//a function that is called in a thread or a child after fork
//this function takes a void pointer and returns a void pointer
void *my_thread(void *arg)
{
    //arg is whatever the argument passed from the thread or the argument given in the call by the child (after fork())
    fprintf(stderr, THREAD_NAME "began, argument is %p\n", arg);

    //sleep for 1 second
    fprintf(stderr, THREAD_NAME "waiting for a while...\n");
    sleep(1);

    //this is the value of the thread or the fork case
    fprintf(stderr, THREAD_NAME "shared was %d\n", shared);
    //shared is assigned 7
    shared = 7;
    //this is the new assigned value of shared which is 7
    fprintf(stderr, THREAD_NAME "shared is now %d\n", shared);

    //sleep for 2 seconds
    fprintf(stderr, THREAD_NAME "waiting a while again...\n");
    sleep(2);

    //this is the value of the thread or the fork case
    fprintf(stderr, THREAD_NAME "shared is now %d\n", shared);

    //sleep for 2 seconds
    fprintf(stderr, THREAD_NAME "waiting once more...\n");
    sleep(2);

    //exiting with the code 7
    fprintf(stderr, THREAD_NAME "exiting with code 7\n");
    //a way to pass an integer through a void pointer
    return (void *) 7;
}

int main() 
{
    fprintf(stderr, "\n\t\tStarting in main(), shared is %d\n", shared); //shared is 3

    {
#ifdef THREAD
        pthread_t thr;
        void *retval;
        //prints the start of thread
        fprintf(stderr, "\t\tCreating new thread with arg 0x12345678...\n");
        //start of thread
        pthread_create(&thr, NULL, my_thread, (void *) 0x12345678);

#else  /* #ifdef FORK */
        int child, status;
        //prints the start of fork()
        fprintf(stderr, "\t\tStarting Fork: Forking new process, calling my_thread with 0x87654321...\n"); 
        //start of fork()
        child = fork();
        //execute child and call my_thread function through child
        if(child == 0)
            exit((int) my_thread((void *) 0x87654321));
#endif

        //prints the previous value of shared which is 3
        fprintf(stderr, "\t\tIn main, shared was %d\n", shared);
        //assigns the shared value to 5
        shared = 5;
        //prints the new value of shared which is 5
        fprintf(stderr, "\t\tIn main, shared is now %d\n", shared);

        //sleeps for 2 seconds
        fprintf(stderr, "\t\tMain sleeps for a bit...\n");
        sleep(2);

        //prints the previous value of shared which is 5
        fprintf(stderr, "\t\tIn main, shared was %d\n", shared);
        //assigns the shared value to 9
        shared = 9;
        //prints the new value of shared which is 9
        fprintf(stderr, "\t\tIn main, shared is now %d\n", shared);

        //sleeps for 2 seconds
        fprintf(stderr, "\t\tMain sleeps for just a bit longer...\n");
        sleep(2);

        //prints the final value of shared which is 9
        fprintf(stderr, "\t\tIn main, final value of shared is now %d\n", shared);

#ifdef THREAD
        //prints wait for the thread to terminate
        fprintf(stderr, "\t\tWaiting for thread via pthread_join...\n");
        //waits for the thread to terminate
        pthread_join(thr, &retval);
        //prints exit code 7 which is the return value of my_thread function
        fprintf(stderr, "\t\tExit code of the thread was %p\n", retval);
#else
        //prints waits for the child to terminate
        fprintf(stderr, "\t\tWaiting for child process to terminate...\n");
        //waits for the child to terminate
        waitpid(child, &status, 0);
        //prints ended with code 7 which is the return value of my_thread function
        fprintf(stderr, "\t\tChild process ended with code %d\n", WEXITSTATUS(status));
#endif
    }

    fprintf(stderr, "\t\tMain ends.\n\n");
    return 0;
}
