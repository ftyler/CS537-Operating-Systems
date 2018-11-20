
// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "writer.h"

void *writer_run(void *arg)
{

    thread_info **tinfo = arg;

    Queue *in_q = (*tinfo)->in_q;

    char *currstring = NULL;

//    while(in_q->terminate == 0)
    while (1)
    {
        currstring = DequeueString(in_q);

        if (currstring == NULL)
        {
            break;
        }

        printf("%s", currstring);
  
        free(currstring);

        currstring = NULL;
    }

    pthread_exit(0);
}
