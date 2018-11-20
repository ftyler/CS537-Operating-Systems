#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options_processing.h"
#include "get_proc_list.h"
#include "parser.h"

/* Directory is found in main and passed here to print
 * whichever flags are active.
 */
void print_proc_info(char *dir_name)
{
    parse_files(dir_name);     
    
        if (s_flag)
            printf(" %c", state_info); 

        if (U_flag)
            printf(" utime=%s", utime);

        if (S_flag)
            printf(" stime=%s", stime);

        if (v_flag)
            printf(" vmemory=%s", vmem);

        if (c_flag)
            printf(" [%s]", cmd);

        printf("\n");

}

/* Processes program args. If "-p" is present, prints process
 * information specific to the given PID. Otherwise, prints 
 * information for all processes belonging to the user.
 */
int main(int argc, char *argv[]) 
{
    process_args(argc, argv);   // get program args

    if (pid != -1) {   // p flag has been set 
        char proc_id[BUFSIZ];
        char dir_name[BUFSIZ] = "/proc/";  // directory starts w/ /proc/    
        sprintf(proc_id, "%d", pid);
        strcat(dir_name, proc_id);
        strcat(dir_name, "/");
        printf("%s:", proc_id); 
        print_proc_info(dir_name);   // prints whichever flags are active 
    } else {
        char** proc_list = get_proc_list();   // gets list of directory names

        for (int i = 0; i < BUFSIZ; i++)      
        {
            if (!strcmp(proc_list[i], ""))   // no more processes
                break;

            // get and print the process info
            char proc_id[BUFSIZ];
            strcpy(proc_id, proc_list[i]);
            strcpy(proc_id, strtok(proc_id, "/proc/"));
            printf("%s:", proc_id);
            print_proc_info(proc_list[i]);

        }

        // free allocated memory
        for (int i = 0; i < BUFSIZ; i++)
        {
            free(proc_list[i]);
        }   
        free(proc_list); 
    }

}
