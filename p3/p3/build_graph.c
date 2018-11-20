// Francesca Tyler
// Bennett Bremer

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "parse_txt.h"
#include "build_graph.h"   
#include "create_execute.h"

// find the buildSpec with given target name
buildSpec* findSpec(buildSpec *curr, char* name)
{ 
    while(curr != NULL)
    {
        if (!strcmp(curr->target, name))
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

// find the specNode with the given target name
specNode *findNode(specNode *firstNode, char *name)
{
    while(firstNode != NULL)
    {
        if (!strcmp(firstNode->spec->target, name))
        {
            return firstNode;
        }
        firstNode = firstNode->next;
    }
    return NULL;
}

// get the last specNode in the linked list of specNodes
specNode *getEndNode(specNode *firstNode)
{
    while (firstNode != NULL)
    {
       if (firstNode->next == NULL)
           return firstNode;
       firstNode = firstNode->next;
    }
    return NULL;
}

//stackTop: current top of stack
//n: specNode to be pushed on to the stak
static listNode* push(listNode *stackTop, specNode *n, listNode *parent)
{
    if (((specNode*) parent->data)->currChild == NULL)
    {
        ((specNode*) parent->data)->currChild = calloc(1, sizeof(listNode));
        ((specNode*) parent->data)->currChild->data = (void*) n;
    }
    else
    {
        listNode *temp = calloc(1, sizeof(listNode));
        temp->data = (void*) n;
        temp->next = ((specNode*) parent->data)->currChild;
        ((specNode*) parent->data)->currChild = temp;
    }
    if (n->ancestor)
    {
        fprintf(stderr, "Error: cyclic dependency involving %s found.\n", n->spec->target);
        exit(1);
    }

    listNode *newST = calloc(0, sizeof(listNode));
    newST->next = stackTop;
    newST->data = (void*) n;
    stackTop = newST;

    return stackTop;
}

static listNode* pop(listNode *stackTop)
{
    listNode *temp = stackTop;
    stackTop = stackTop->next;
    free(temp);
    return stackTop;
}

static listNode* eval(buildSpec *firstSpec, specNode *firstNode, listNode *stackTop)
{
    specNode *stackTopSN = (specNode*) stackTop->data;    
    if (stackTopSN->seen || !should_exec(stackTopSN))
    {
        stackTopSN->ancestor = 0;
        stackTop = pop(stackTop);
        return stackTop;
    }

    listNode *parent = stackTop;  
    int pushed_children = 0;
    stackTopSN->seen = 1;
    listNode *currDependency = stackTopSN->spec->firstDependency; //children/dependencies
    specNode *currNode = NULL;                //node w/ current child as spec target
    while(currDependency != NULL)
    {
        currNode = findNode(firstNode, currDependency->data);
        if (currNode != NULL)
        {
            stackTop = push(stackTop, currNode, parent);
            pushed_children++;
        }
        else
        {
            buildSpec *currSpec = findSpec(firstSpec, currDependency->data);        //spec of current child as target
            if (currSpec != NULL)
            {
                currNode = calloc(1, sizeof(specNode));
                currNode->spec = currSpec;
                currNode->prev = getEndNode(firstNode);
                currNode->seen = 0;
                currNode->ancestor = 0;
                getEndNode(firstNode)->next = currNode;
                stackTop = push(stackTop, currNode, parent);
                pushed_children++;
            }
            else
            {
                if (access(currDependency->data, F_OK) == -1)
                {
                    fprintf(stderr, "Error: File %s not found.\n", (char*) currDependency->data);
                }
            }
        }
        currDependency = currDependency->next;
    }
    if (!pushed_children)
    {
            stackTopSN->ancestor = 0;
            stackTop = pop(stackTop);
    }
    else
    {
        stackTopSN->ancestor = 1;
    } 

    return stackTop;
}

specNode* build_graph(buildSpec *first, char* rootName)
{
    // set the front of queue to the root node
    specNode *rootNode = calloc(1, sizeof(specNode));
    rootNode->spec = findSpec(first, rootName);

    if (rootNode->spec == NULL)
    {
        fprintf(stderr, "Error: Cannot find root node: %s\n", rootName);
    }

    if (!should_exec(rootNode))
    {
        printf("%s is up to date\n", rootNode->spec->target);
        exit(1);
    }

    // add root dependencies to graph
    listNode *stackTop = calloc(1, sizeof(listNode));
    stackTop->data = (void *) rootNode;

    while (stackTop != NULL)
    {
        stackTop = eval(first, rootNode, stackTop);
    }

    return getEndNode(rootNode);
}
