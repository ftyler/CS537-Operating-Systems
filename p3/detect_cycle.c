// Francesca Tyler
// Bennett Bremer

#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "parse_txt.h"
#include "build_graph.h"   
  
void free_stack(listNode *stackTop)
{
    listNode *temp = NULL;
    while (stackTop != NULL)
    {
        temp  = stackTop;
        stackTop = stackTop->next;
        free(temp);
    }
}

//stackTop: current top of stack
//n: specNode to be pushed on to the stack
static listNode* push(listNode *stackTop, specNode *n)
{
    n->spec->inGraph = 1; // notify that n->spec has been check for cyclical dependencies

    // n is already an ancestor, therefore a cyclical dependency exists
    if (n->ancestor)
    {
        fprintf(stderr, "Error: cyclic dependency involving %s found.\n", n->spec->target);
//        listNode *temp = stackTop;
//        stackTop = stackTop->next;
//        free(stackTop);
//        exit(1);

        free_stack(stackTop);
        return NULL;
        
    }

    // make a list node with n and put it on stack
    listNode *newST = calloc(1, sizeof(listNode));
    newST->next = stackTop;
    newST->data = (void*) n;
    stackTop = newST;

    return stackTop;
}

// take the top off the stack
static listNode* pop(listNode *stackTop)
{
    listNode *temp = stackTop;
    stackTop = stackTop->next;
    free(temp);
    return stackTop;
}

// push all valid children of stackTop to stack
static listNode* eval(buildSpec *firstSpec, specNode *firstNode, listNode *stackTop)
{
    // if stackTop has been seen, pop it and return
    specNode *stackTopSN = (specNode*) stackTop->data;    
    if (stackTopSN->seen)
    {
        stackTopSN->ancestor = 0;
        stackTop = pop(stackTop);
        return stackTop;
    }

    int pushed_children = 0; // number of children that have been push
    stackTopSN->seen = 1;    // mark that stackTop has been seen

    // iterate through dependencies, check if file exists or push
    listNode *currDependency = stackTopSN->spec->firstDependency;
    specNode *currNode = NULL;                //node w/ current child as spec target
    while(currDependency != NULL)
    {
        // check if dependency already exists as node
        currNode = findNode(firstNode, currDependency->data);
        if (currNode != NULL)
        {
            stackTop = push(stackTop, currNode);
            if (stackTop == NULL) // check for error return value
            {
                specNode* temp = NULL;
                while (firstNode != NULL)
                {
                    temp = firstNode;
                    firstNode = firstNode->next;
                    free(temp);
                }
                free_buildSpec_list(firstSpec);
                exit(1);
            }
            else
            {
                pushed_children++;
            }
        }
        else
        {
            // check if dependency was parsed from makefile as buildspec
            buildSpec *currSpec = findSpec(firstSpec, currDependency->data);
            if (currSpec != NULL)
            {
                // if spec found, make new node and push
                currNode = calloc(1, sizeof(specNode));
                currNode->spec = currSpec;
                currNode->prev = getEndNode(firstNode);
                currNode->seen = 0;
                currNode->ancestor = 0;
                getEndNode(firstNode)->next = currNode;
                stackTop = push(stackTop, currNode);
                if (stackTop == NULL) // error, free memory
                {
                    specNode *temp = NULL;
                    while (firstNode != NULL)
                    {
                        temp = firstNode;
                        firstNode = firstNode->next;
                        free(temp);
                    }
                    free_buildSpec_list(firstSpec);
                    exit(1);
                }
                pushed_children++;
            }
            else
            {
                // otherwise file does not exists error
                if (access(currDependency->data, F_OK) == -1)
                {
                    fprintf(stderr, "Error: File %s not found.\n", (char*) currDependency->data);
                    while (stackTop != NULL)
                    {
                        free_stack(stackTop);
                    }
                    specNode *temp1 = NULL;
                    while (firstNode != NULL)
                    {
                        temp1 = firstNode;
                        firstNode = firstNode->next;
                        free(temp1);
                    }
                    free_buildSpec_list(firstSpec);
                    exit(1);
                }
            }
        }
        currDependency = currDependency->next;
    }

    // if no children were pushed, pop the stackTop
    if (!pushed_children)
    {
            stackTopSN->ancestor = 0;
            stackTop = pop(stackTop);
    }
    // if children were pushed, old stackTop is now an ancestor
    else
    {
        stackTopSN->ancestor = 1;
    } 

    return stackTop;
}

// build a graph from given root, checking for cycles
void build_from_root(buildSpec *first, buildSpec *root)
{
    // set the front of queue to the root node
    specNode *rootNode = calloc(1, sizeof(specNode));
    rootNode->spec = root;

    // add root dependencies to graph
    listNode *stackTop = calloc(1, sizeof(listNode));
    stackTop->data = (void *) rootNode;

    // eval stackTop until stack exhausted
    while (stackTop != NULL)
    {
        stackTop = eval(first, rootNode, stackTop);
    }

    specNode *temp = NULL;
    while (rootNode != NULL)
    {
        temp = rootNode;
        rootNode = rootNode->next;
        free(temp);
    }
}

// check graphs given all possible root nodes for cycles
void detect_cycle(buildSpec *first)
{
    buildSpec *curr = first;
    while (curr != NULL)
    {
        if (!curr->inGraph)
        {
            build_from_root(first, curr);
        }
        curr = curr->next;
    }
}
