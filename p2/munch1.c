// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <pthread.h>
#include <stdio.h>

#include "queue.h"
#include "munch1.h"

/*  munch1 will dequeue the string from the in_q from reader. It will
 *  change each space (' ') char to '*'. It will then enqueue this 
 *  string for the out_q that will be passed into munch2.
 */
void *munch1_run(void *arg)
{
    thread_info **tinfo = arg;  // instance of a struct that contains a queue

    Queue *in_q = (*tinfo)->in_q;    // queue passed in from reader
    Queue *out_q = (*tinfo)->out_q;  // out queue to pass to munch2

    char *currstring = NULL;  // where input string will be stored

    while (1) {  // continuously dequeues, breaks when there are no more strings
        currstring = DequeueString(in_q);  

        if (currstring == NULL)  // breaks if a null char is reached in the queue
        {
            break; 
        }

        int i = 0;
        // goes through string until a newline is reached
        while (currstring[i] != '\n') 
        {
            if (currstring[i] == ' ')  // replaces space char with '*'.
                currstring[i] = '*';
            i++;
        }

        EnqueueString(out_q, currstring);  // enqueues modified string for munch2
    }

    EnqueueString(out_q, NULL);  // enqueues a null char at the end of the queue
            

    pthread_exit(0);
}
