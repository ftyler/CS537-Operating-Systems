// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "queue.h"


Queue *CreateStringQueue(int size) 
{
    Queue *Q = (Queue*) malloc(sizeof(Queue)); 
    Q->buffer = (char **)malloc(sizeof(char*)*size);
    Q->end = 0; 
    Q->size = size;
    Q->enqueueCount = 0;
    Q->dequeueCount = 0;
    Q->enqueueBlockCount = 0;
    Q->dequeueBlockCount = 0; 

    sem_init(&Q->empty, 0, size);
    sem_init(&Q->full, 0, 0);
    sem_init(&Q->mutex, 0, 1);

    return Q; 
}

void EnqueueString(Queue *Q, char *string)
{

    sem_wait(&Q->mutex);
    if (Q->end == Q->size)
        (Q->enqueueBlockCount)++;
    sem_post(&Q->mutex);
    sem_wait(&Q->empty);

    sem_wait(&Q->mutex);
    Q->buffer[Q->end] = string;
    (Q->end)++;
    (Q->enqueueCount)++;
    sem_post(&Q->mutex);
    sem_post(&Q->full);
}

char *DequeueString(Queue *Q)
{
    sem_wait(&Q->mutex);
    if (Q->end == 0)
        (Q->dequeueBlockCount)++;
    sem_post(&Q->mutex);
    sem_wait(&Q->full);
    
    sem_wait(&Q->mutex);
    char *ret = Q->buffer[0];
    (Q->end)--;
    for (int i = 0; i < Q->end; i++)
    {   
        Q->buffer[i] = Q->buffer[i+1];
    }
    (Q->dequeueCount)++;
    sem_post(&Q->mutex);
    sem_post(&Q->empty);

    return ret;
}

void PrintQueueStats(Queue *Q)
{
    printf("enqueueCount: %i\n", Q->enqueueCount);
    printf("dequeueCount: %i\n", Q->dequeueCount);
    printf("enqueueBlockCount: %i\n", Q->enqueueBlockCount);
    printf("dequeueBlockCount: %i\n", Q->dequeueBlockCount);
}
