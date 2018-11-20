// Francesca Tyler
// Bennett Bremer

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "create_execute.h"

void print_cmd(char **argv)
{
    int i = 0;
    while (argv[i] != NULL)
    {
        printf("arg: \"%s\"\n", argv[i]);
        i++;
    }
    printf("\n");
}

char** split(char *cmd)
{
    char* cpy = calloc(strlen(cmd)+1, sizeof(char));
    strcpy(cpy, cmd);
    listNode *first_arg = calloc(1, sizeof(listNode));
    first_arg->data = (void*) cpy;
    listNode *curr_arg = first_arg;
    int ind = 1;
    int numStrings = 1;
    while (cpy[ind] != '\0')
    {
        if (cpy[ind-1] == ' ')
        {
            cpy[ind-1] = '\0';
            curr_arg->next = calloc(1, sizeof(listNode));
            curr_arg = curr_arg->next;
            curr_arg->data = (void*) &cpy[ind];
            numStrings++;
        }

        ind++;
    }
    char **argv = calloc(numStrings+6, sizeof(char*));
    ind = 0;
    curr_arg = first_arg;
    while (curr_arg != NULL)
    {
        argv[ind] = (char*)curr_arg->data;
        curr_arg = curr_arg->next;
        ind++;
    }

    argv[ind] = NULL;

    return argv;
}

int should_exec(specNode *targetNode)
{
    struct stat *b = malloc(sizeof(struct stat));
    int exists = stat(targetNode->spec->target, b);
    long int target_t = 0;
    if (exists)
    {
        free(b);
        return 1;
    }
    else
    {
        target_t = b->st_mtime;
    }

    listNode *currChild = targetNode->spec->firstDependency;
    while (currChild != NULL)
    {
        int exists = stat((char*)currChild->data, b);
        if (exists || b->st_mtime > target_t)
        {
            free(b);
            return 1;
        }
        currChild = currChild->next;
    }

    free(b);
    return 0;
}

int command(specNode *targetNode) 
{
    pid_t pid = 0;

    listNode *currCommand = targetNode->spec->firstCommand;
    while (currCommand != NULL)
    {
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Fork error.\n");
            exit(1);
        }
        else if (pid > 1)
        {
            pid = waitpid(pid, NULL, 0);
            if (pid == -1)
            {
                fprintf(stderr, "wait error\n");
                exit(1);
            }
        }
        else
        {
            char **argv = split((char*)currCommand->data);
            execvp(argv[0], argv);
            printf("error after execvp has occurred\n");
            exit(1);
        }
        currCommand = currCommand->next;
    }
    return 0;
}
