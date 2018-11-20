// Francesca Tyler
// Bennett Bremer

#include "parse_txt.h"

#ifndef guard_graph
#define guard_graph

typedef struct specNode
{
    buildSpec *spec;
    struct specNode *prev;
    struct specNode *next;
    listNode *currChild;
    int seen;
    int ancestor;
} specNode;

buildSpec* findSpec(buildSpec*, char*);

specNode* findNode(specNode*, char*); 

specNode *getEndNode(specNode*);

specNode* build_graph(buildSpec*, char*);

#endif
