
// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <semaphore.h>

#ifndef guard_queue
#define guard_queue

typedef struct
{
    char **buffer;
    int end;   // keep track of the end of the queue
    sem_t empty, full, mutex;
    int size;  // size of queue, same as num of strings

    // ints to keep track of stats
    int enqueueCount;
    int dequeueCount;
    int enqueueBlockCount;
    int dequeueBlockCount;
} Queue;

typedef struct
{
    Queue *in_q;
    Queue *out_q;
} thread_info;

Queue *CreateStringQueue(int);

void EnqueueString(Queue*, char*);

char *DequeueString(Queue*);

void PrintQueueStats(Queue*);

#endif
