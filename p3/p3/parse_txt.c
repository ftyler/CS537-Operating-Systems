// Francesca Tyler
// Bennett Bremer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse_txt.h"

const int buffer = 1024;

// prints information for given build
void print_build(buildSpec *build)
{
    // print target
    printf("target: \"%s\"\n", build->target);

    //print all dependencies
    listNode *curr = build->firstDependency;
    while (curr != NULL)
    {
        printf("dependency: \"%s\"\n", (char*) curr->data);
        curr = curr->next;
    }

    //print all commands
    curr = build->firstCommand;
    while (curr != NULL)
    {
        printf("command: \"%s\"\n", (char*) curr->data);
        curr = curr->next;
    }

    printf("\n");
}

// finds if targetName has duplicate target in buildSpec list starting at input curr
void duplicateTarget(buildSpec *curr, char *targetName)
{
    while (curr != NULL)
    {
        // exclude last node: itself
        if (!strcmp(curr->target, targetName) && curr->next != NULL)
        {
            fprintf(stderr, "Error: Duplicate target '%s' found.\n", targetName);
            exit(1);
        }
        curr = curr->next;
    }
}

// initializes a buildSpec
buildSpec* new_buildSpec()
{
    buildSpec* build = malloc(sizeof(buildSpec));
    build->target = NULL;
    build->firstDependency = NULL;
    build->firstCommand = NULL;
    build->next = NULL;
    return build;
}

// parses target line currLine into build
void parse_target_line(buildSpec *build, char *currLine, int lineNum)
{
    int len = strlen(currLine);    // length of whole line
    build->target = currLine;      // set target to begin at currLine start
    int strPos = 1;                // current position in string

    // get target name
    int targetFinished = 0;        // still parsing target?
    while(1)
    {
        // check that target is whole line, i.e. no colon in line
        if (strPos >= len)
        {
            fprintf(stderr, "%i: Invalid line: \"%s\"\n", lineNum, currLine);
            exit(1);
        }
 
        // if space found, target is finished, set to null for string termination
        if (currLine[strPos] == ' ')
        {
            currLine[strPos] = '\0';
            if (!targetFinished)
                targetFinished = 1;
        }

        // if colon found, done parsing target
        else if (currLine[strPos] == ':')
        {
            if (strPos + 1 >= len)
            {
                currLine[strPos] = '\0';
                return;
            }
            currLine[strPos] = '\0';
            strPos++;
            break;
        }

        // otherwise, if done parsing target then exists space separated 
        // strings before columns, which is invalid
        else
        {
            if (targetFinished)
            {
                fprintf(stderr, "%i: Invalid line: \"%s\"\n", lineNum, currLine);
                exit(1);
            } 
        }

        strPos++;
    }

    //get dependencies
    char* temp_dependency = NULL; // pointer to beginning of curr dependency string
    listNode *temp_ln = NULL;     // listNode of curr dependency, constructs list
    while(1)
    {
        // if space found, change to null for string termination and move on
        if (currLine[strPos] == ' ')
        {
            currLine[strPos] = '\0';
            strPos++;
            continue;
        }

        // if colon found, it must be second colon (b/c target parsing)
        if (currLine[strPos] == ':')
        {
            fprintf(stderr, "%i: Invalid line: \"%s\"\n", lineNum, currLine);
            exit(1);
        }

        // if null found, reached end of currLine
        if (currLine[strPos] == '\0')
        {
            break;
        }

        // if no above chars found, begin reading dependency
        temp_dependency = &currLine[strPos];
        while(1)
        {

            // if colon found, it must be second colon (b/c target parsing)
            if (currLine[strPos] == ':')
            {
                fprintf(stderr, "%i: Invalid line: \"%s\"\n", lineNum, currLine);
                exit(1);
            }

            // if null found, reached end of currLine
            if (currLine[strPos] == '\0')
            {
                break;
            }

            // if space found, end of dependency
            if (currLine[strPos] == ' ')
            {
                currLine[strPos] = '\0';
                strPos++;
                break;
            }

            strPos++;
        }

        // if build has no dependencies yet, make curr dependency firstDependency
        if (build->firstDependency == NULL)
        {
            build->firstDependency = malloc(sizeof(listNode));
            build->firstDependency->data = temp_dependency;
            temp_ln = build->firstDependency;
        }

        // else put curr dependency on dependency list
        else
        {
            temp_ln->next = malloc(sizeof(listNode));
            temp_ln = temp_ln->next;
            temp_ln->data = temp_dependency;
        }

        if (currLine[strPos] == '\0')
        {
            break;
        }
    }
}

//iterate through each line of file and handle
buildSpec* parse_file(FILE* f)
{
    buildSpec *header = new_buildSpec(); // first buildSpec in makefile
    buildSpec *build = header;           // current buildSpec
    
    // filtered line reader from file
    char* currLine = malloc(sizeof(char)*buffer);
    int linePos = 0;  // current position in line
    int currChar = 0; // current character in line
    int lineNum = 0;  // current line number

    // used to build command list on current build
    listNode* currCommand = NULL;

    // iterate through all characters in file
    while(currChar != EOF)
    {
        linePos = 0;

        lineNum++;

        // iterate through characters in single line
        while(1)
        {
            // if line is too long, error
            if (linePos >= buffer - 1)
            {
                fprintf(stderr, "%d: Error: line too long.\n", lineNum);
                exit(1);
            }

            // get current character
            currChar = fgetc(f);
 
            if (currChar == '\0')
            {
                fprintf(stderr, "Null character detected in line %i\n", lineNum);
                exit(1);
            }
         
            // if at end of file, stop iterating
            if (currChar == EOF)
                break;

            // if line is done, break
            if (currChar == '\n')
                break;

            // read character into currLine and move to next
            currLine[linePos++] = currChar;
        }
    
        // terminate string
        currLine[linePos] = '\0';

        // ignore line if blank line, EOF, or comment
        if (currLine[0] == '\n' || currLine[0] == '\0' || currLine[0] == '#')
        {
            continue;
        } 

        // if line starts with alpha or digit, handle as target line 
        if (isalpha(currLine[0]) || isdigit(currLine[0]))
        {
            // make new buildSpec if build target already specified
            if (build->target != NULL)
            {
                build->next = new_buildSpec();
                build = build->next;
            }
            parse_target_line(build, currLine, lineNum); // handle line
            duplicateTarget(header, build->target);      // check for duplicate targets
            currLine = malloc(sizeof(char)*buffer);
        }

        // if line starts with tab, handle as command line
        else if (currLine[0] == '\t')
        {
            // if target not yet specific, orphan command line
            if (build->target == NULL)
            {
                fprintf(stderr, "%i: Invalid line: %s\n", lineNum, currLine);
                exit(1);
            }

            // if build has no commands, make currLine the first
            if (build->firstCommand == NULL)
            {
                build->firstCommand = malloc(sizeof(listNode));
                build->firstCommand->data = &currLine[1]; // ignore tab
                currCommand = build->firstCommand;
            }

            // else, add to command list
            else
            {
                currCommand->next = malloc(sizeof(listNode));
                currCommand = currCommand->next;
                currCommand->data = &currLine[1]; // ignore tab
            }
            
            currLine = malloc(sizeof(char)*buffer);
        }
        else
        {
            fprintf(stderr, "%i: Invalid line: %s\n", lineNum, currLine);
            exit(1);
        }

    }       

    // return the first buildSpec in makefile
    return header;
}
