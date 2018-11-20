// Francesca Tyler, NetID: ftyler, CS login: francesca
// Bennett Bremer, NetID: bbremer, CS login: bbremer

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "queue.h"
#include "reader.h"

/*  Reads from stdin line by line, taking each line and passing it through 
 *  a queue of char strings to the Munch1 thread.
 */
void *reader_run(void *arg)
{
    thread_info **tinfo = arg;  // instance of a struct that contains a queue

    Queue *out_q = (*tinfo)->out_q;  // queue to pass to Munch1

    char *str = NULL;  // where input string will be stored

    const int buffer_size = 1024;  // size of buffer 

    int input;
    int curr_ind = 0;
    str = malloc(sizeof(char)*buffer_size);

    if (str == NULL)
    {
        fprintf(stderr, "Error: No memory space remaining.");
        EnqueueString(out_q, NULL);
        pthread_exit(0);
    }

    memset(str, '\0', buffer_size);

    int tooLong = 0;

    while (1)
    {
        input = getchar();

        if (input == EOF)
        {
            if (str[0] != '\0')
            { 
                //str[curr_ind] = '\n';
                EnqueueString(out_q, str);
            }
            else
            {
                free(str);
            }
            EnqueueString(out_q, NULL);
            break;
        }

        if (input == '\0')
        {
            fprintf(stderr, "Error: Null character detected. Terminating. \n");
            EnqueueString(out_q, NULL);
            break;
        }

        if (tooLong)
        {
            if (input == '\n')
            {
                tooLong = 0;
            }
            continue;
        }

        if (curr_ind >= buffer_size - 1)
        {
            fprintf(stderr, "Error: Line length exceeds buffer size\n");
            if (input != '\n')
            {
                tooLong = 1;
            }
            memset(str, '\0', buffer_size);
            curr_ind = 0;
        }
        else if (input == '\n')
        {
            str[curr_ind++] = input;
            EnqueueString(out_q, str);
            str = malloc(sizeof(char)*buffer_size);
            if (str == NULL)
            {
                fprintf(stderr, "Error: No memory space remaining.");
                EnqueueString(out_q, NULL);
                pthread_exit(0);
            }
            memset(str, '\0', buffer_size);
            curr_ind = 0;
        } 
        else
        {
            str[curr_ind++] = input;
        }
     }

    pthread_exit(0);
}
