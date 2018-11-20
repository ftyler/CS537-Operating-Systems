#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "get_proc_list.h"
#include "check_numeric.c"

/* Used to look through /proc to find list of processes
 * belonging to user, returns list of directory names
 */
char** get_proc_list()
{
    char *proc_s = "/proc";    // directory name      
    DIR *s = opendir(proc_s);  // opens /proc directory stream

    struct dirent *proc_dir;   // directory entries in /proc

    char full_dir_name[BUFSIZ];   // absolute addr of a directory in /proc

    char full_status_name[BUFSIZ];   // absolute addr of status file in /proc

    FILE *status_file;   // status files

    char buffer[BUFSIZ];  

    // list of directories in /proc
    char **process_list = calloc(BUFSIZ, sizeof(char*)); 

    // allocate memory for directories
    for (int i = 0; i < BUFSIZ; i++)   
    {
        process_list[i] = calloc(BUFSIZ, sizeof(char));
    }

    uid_t euid = geteuid();

    int curr_ind = 0;

    while((proc_dir = readdir(s)))   // get next dir in /proc
    {
        // continue if directory name d_name is not a pid
        if (!(isNumeric(proc_dir->d_name)))   
             continue;

	memset(full_dir_name, '\0', sizeof(full_dir_name));   // empty full_dir_name   
        strcpy(full_dir_name, proc_s);   // copies '/proc' portion into full_dir_name
        strcat(full_dir_name, "/");      // appends '/' for correct directory format
        strcat(full_dir_name, proc_dir->d_name);   // adds on rest of directory name
        strcat(full_dir_name, "/");   // appends '/' to finish correct directory format

        // empty full_status_name
	memset(full_status_name, '\0', sizeof(full_status_name));
        // actually create the absolue addr of status file
	strcat(strcpy(full_status_name, full_dir_name), "status");  

	if ((status_file = fopen(full_status_name, "r")))
	{
            // read lines from file to find Uid line
	    while (fgets(buffer, BUFSIZ, status_file) != NULL)
            {
		if (!strcmp(strtok(buffer, "\t"), "Uid:"))
	        {
	            strtok(NULL, "\t");
	            if ((uid_t) atoi(strtok(NULL, "\t")) == euid)
	            {
			strcpy(process_list[curr_ind], full_dir_name);
		        curr_ind++;
		    }
                }
    	    }
	}
	fclose(status_file);
    }
    closedir(s);

    return process_list;
}

