// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <pthread.h>
#include <stdio.h>

#include "queue.h"
#include "munch2.h"

void *munch2_run(void *arg)
{

    thread_info **tinfo = arg;

    Queue *in_q = (*tinfo)->in_q;
    Queue *out_q = (*tinfo)->out_q;

    char *currstring = NULL;

//    while(in_q->terminate == 0)
    while (1)
    {
        currstring = DequeueString(in_q);

        if (currstring == NULL)
            break;

        int i = 0;
        while (currstring[i] != '\n')
        {
            if (currstring[i] >= 'a' && currstring[i] <= 'z')
                currstring[i] = currstring[i] - 32;
            i++;
        }

        EnqueueString(out_q, currstring);

    }

    EnqueueString(out_q, NULL);

    pthread_exit(0);
}
