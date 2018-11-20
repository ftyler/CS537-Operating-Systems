// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"
#include "reader.h"
#include "munch1.h"
#include "munch2.h"
#include "writer.h"


/*  Creates three queues, one between each of the threads.
 *  Creates each of the threads and prints the queue stats.
 */
int main() 
{
    int size = 10;  // used for queue size

    // initialization of queues
    Queue *r_m1_q = CreateStringQueue(size);
    Queue *m1_m2_q = CreateStringQueue(size);
    Queue *m2_w_q = CreateStringQueue(size);

    // malloc space for queues
    thread_info *r_info = malloc(sizeof(thread_info));
    thread_info *m1_info = malloc(sizeof(thread_info));
    thread_info *m2_info = malloc(sizeof(thread_info));
    thread_info *w_info = malloc(sizeof(thread_info));

    if (r_info == NULL || m1_info == NULL || m2_info == NULL || w_info == NULL)
    {
        fprintf(stderr, "Error: No memory space remaining.");
        exit(1);
    }

    r_info->out_q = r_m1_q;  // reader's outqueue to pass to Munch1

    m1_info->in_q = r_m1_q;  // Munch1's inqueue from reader
    m1_info->out_q = m1_m2_q;  // Munch1's outqueue to Munch2

    m2_info->in_q = m1_m2_q;  // Munch2's inqueue from Munch1
    m2_info->out_q = m2_w_q;  // Munch2's outqueue to writer

    w_info->in_q = m2_w_q;  // Writer's inqueue from Munch2

    // used for thread IDs
    pthread_t r;
    pthread_t m1;
    pthread_t m2;
    pthread_t w;

    // actually creating threads
    int ptc_check;

    ptc_check = pthread_create(&w, NULL, writer_run, &w_info);
    if (ptc_check)
    {
        fprintf(stderr, "Error: Thread could not be created.");
        exit(1);
    } 

    ptc_check = pthread_create(&m2, NULL, munch2_run, &m2_info); 
    if (ptc_check)
    {
        fprintf(stderr, "Error: Thread could not be created.");
        EnqueueString(m2_w_q, NULL);
        exit(1);
    }

    ptc_check = pthread_create(&m1, NULL, munch1_run, &m1_info); 
    if (ptc_check)
    {
        fprintf(stderr, "Error: Thread could not be created.");
        EnqueueString(m1_m2_q, NULL);
        exit(1);
    }
    

    ptc_check = pthread_create(&r, NULL, reader_run, &r_info); 
    if (ptc_check)
    {
        fprintf(stderr, "Error: Thread could not be created.");
        EnqueueString(r_m1_q, NULL);
        exit(1);
    }

    void **ret = NULL;  // pointer for thread to use for queue

    ptc_check = pthread_join(w, ret);
    if (ptc_check)
    {
        fprintf(stderr, "Error with pthread_join().");
        exit(1);
    }

    ptc_check = pthread_join(m2, ret);
    if (ptc_check)
    {
        fprintf(stderr, "Error with pthread_join().");
        EnqueueString(m2_w_q, NULL);
        exit(1);
    }

    ptc_check = pthread_join(m1, ret);
    if (ptc_check)
    {
        fprintf(stderr, "Error with pthread_join().");
        EnqueueString(m1_m2_q, NULL);
        exit(1);
    }

    ptc_check = pthread_join(r, ret);
    if (ptc_check)
    {
        fprintf(stderr, "Error with pthread_join().");
        EnqueueString(r_m1_q, NULL);
        exit(1);
    }

    // Each call will print their respective thread's stats, 
    // depending on the arg
    PrintQueueStats(r_m1_q);
    PrintQueueStats(m1_m2_q);
    PrintQueueStats(m2_w_q);

    // free buffers
    free(r_m1_q->buffer);
    free(m1_m2_q->buffer);
    free(m2_w_q->buffer);

    // free queues and null pointers
    free(r_m1_q);
    r_m1_q = NULL;
    free(m1_m2_q);
    m1_m2_q = NULL;
    free(m2_w_q);
    m2_w_q = NULL;
    free(r_info);

    // free threads and null pointers
    r_info = NULL;
    free(m1_info);
    m1_info = NULL;
    free(m2_info);
    m2_info = NULL;
    free(w_info);
    w_info = NULL;

    return 1;
}
