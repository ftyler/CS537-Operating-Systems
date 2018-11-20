// Francesca Tyler
// Bennett Bremer

#ifndef guard_parse
#define guard_parse

#include <stdio.h>

/*typedef struct strNode
{
    char *str;
    struct strNode *next;
} strNode;*/

typedef struct listNode
{
    void *data;
    struct listNode *next;
} listNode;

typedef struct buildSpec
{
    char *target;
    listNode *firstDependency;
    listNode *firstCommand;
    struct buildSpec *next;
    int inGraph;
} buildSpec;

buildSpec* parse_file(FILE*);

void free_buildSpec_list(buildSpec*);


#endif
