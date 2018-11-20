#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parser.h"
#include "options_processing.h"

char state_info = '\0';  // status information
char utime[BUFSIZ];   // user time
char stime[BUFSIZ];   // system time
char vmem[BUFSIZ];    // virtual memory usage
char cmd[BUFSIZ];     // command line info

char buffer[BUFSIZ];

char* parse_cmd(FILE *cmd_file)
{
    fgets(buffer, BUFSIZ, cmd_file);
    for (int i = 0; i < BUFSIZ - 1; i++)
    {
	if (buffer[i] == '\0' && buffer[i+1] != '\0')
	{
	    buffer[i] = 32;
	}
    }
    return buffer;
}

/* Gets strings from lists read from stat and statm files */
void parse_files(char *dir_path)
{
   // clear out arrays
   memset(utime, '\0', BUFSIZ);
   memset(stime, '\0', BUFSIZ);
   memset(vmem, '\0', BUFSIZ);
   memset(cmd, '\0', BUFSIZ);

   if (s_flag || U_flag || S_flag)
   {
       char state_temp[BUFSIZ];   // placeholder for state char
       FILE *stat_file;

       // s, U and S flags get info from stat file
       if ((stat_file = fopen(strcat(strcpy(state_temp, dir_path), "stat"), "r")))
       {
           // parses stat file by spaces
           strtok(fgets(buffer, BUFSIZ, stat_file), " ");
           strtok(NULL, " ");

           if (s_flag)   // update state_temp 
           {
               strcpy(state_temp, strtok(NULL, " "));
               state_info = state_temp[0];
           } else {   // continue parsing
               strtok(NULL, " ");
           }

           if (U_flag)   // update utime
           {
               strcpy(utime, strtok(NULL, " "));
           } else {   // continue parsing
               strtok(NULL, " ");
           }

           if (S_flag)   // update stime
           {
               strcpy(stime, strtok(NULL, " "));
           }
       }
   }

   if (v_flag)   // update vmem
   {
       char vmem_fn[BUFSIZ];   // vmem file name
       FILE *statm_file;       

       // v flag gets info from statmf file
       if ((statm_file = fopen(strcat(strcpy(vmem_fn, dir_path), "statm"), "r")))
       {
	   strcpy(vmem, strtok(fgets(buffer, BUFSIZ, statm_file), " "));
       }
   }

   if (c_flag)   // update cmd
   {
       char cmd_fn[BUFSIZ];
       FILE *cmd_file;

       // c flag gets info from cmdline file
       if ((cmd_file = fopen(strcat(strcpy(cmd_fn, dir_path), "cmdline"), "r")))
       {
           strcpy(cmd, parse_cmd(cmd_file));
       } 
       else 
       {
           printf("File error");
       }
   }
}
