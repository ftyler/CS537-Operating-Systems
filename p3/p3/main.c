// Francesca Tyler
// Bennett Bremer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_txt.h"
#include "build_graph.h"
#include "detect_cycle.h"
#include "create_execute.h"

int main(int argc, char *argv[])
{
    FILE *f;   
    char *inTarget = NULL;  // specify input target

    // handle args by number of args given
    if (argc == 1)
    {
        f = fopen("makefile", "r");
        if (f == NULL)
            f = fopen("Makefile", "r");
        if (f == NULL)
        {
            fprintf(stderr, "Error: makefile cannot be found\n");
            exit(1);
        }
    }
    else if (argc == 2)
    {
        if (!strcmp(argv[1], "-f"))
        {
            fprintf(stderr, "Error: must specify valid file\n");
            exit(1);
        }
        inTarget = argv[1];
        f = fopen("makefile", "r");
        if (f == NULL)
            f = fopen("Makefile", "r");
        if (f == NULL)
        {
            fprintf(stderr, "Error: makefile cannot be found\n");
            exit(1);
        }
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], "-f"))
        {
            fprintf(stderr, "Error: Usage: 537make [target] [-f filename]\n");
            exit(1);
        }
        f = fopen(argv[2], "r");
        if (f == NULL)
        {
            fprintf(stderr, "Error: %s cannot be found.\n", argv[2]);
            exit(1);
        }
    }
    else if (argc == 4)
    {
        if (!strcmp(argv[1], "-f"))
        {
            f = fopen(argv[2], "r");
            if (f == NULL)
            {
                fprintf(stderr, "Error: %s cannot be found.\n", argv[2]);
                exit(1);
            }
            inTarget = argv[3];
        }
        else if (!strcmp(argv[2], "-f"))
        {
            inTarget = argv[1];
            f = fopen(argv[3], "r");
            if (f == NULL)
            {
                fprintf(stderr, "Error: %s cannot be found.\n", argv[3]);
                exit(1);
            }
        }
        else 
        {
            fprintf(stderr, "Error: Usage: 537make [target] [-f filename]\n");
            exit(1);
        }
    }

    // parse the file and close, first is first build in makefile
    buildSpec *first = parse_file(f);
    fclose(f);

    //check that in target is found in buildspecs
    if (inTarget == NULL)
    {
        inTarget = first->target;
    }
    else
    {
        int targetFound = 0;
        buildSpec *curr = first;
        while(curr != NULL)
        {
            if (!strcmp(curr->target, inTarget))
            {
                targetFound = 1;
                break;
            }
            curr = curr->next;
        }
        if (!targetFound)
        {
            fprintf(stderr, "Error: target \"%s\" not found.\n", inTarget);
            exit(1);
        }
    }

    // detect any cycles in makefile (including those outside target)
    detect_cycle(first);

    // build execution order using graph traversal
    specNode* currTarget = build_graph(first, inTarget);

    //execute all commands in order of built graph
    while (currTarget != NULL)
    {
        command(currTarget);
        currTarget = currTarget->prev;
    }

    return 0;
}
