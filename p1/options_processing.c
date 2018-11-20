#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "options_processing.h"
#include "check_numeric.h"

// sets T/F flags for information display
int pid = -1;     
int s_flag = 0;
int U_flag = 1;
int S_flag = 0;
int v_flag = 0;
int c_flag = 1;

/* Determines what flags to set depending on argv elements. */
void process_args(int argc, char *argv[]) 
{
    char option;    // argument options for user input  

    while (-1 != (option = getopt(argc, argv, "p:s::U::S::v::c::")))
    {
        switch (option)
	{
            case 'p':
		if (isNumeric(optarg))    // ensures pid is numeric
		{
		    pid = atoi(optarg);
                    char fn[BUFSIZ] = "/proc/";
                    strcat(fn, optarg);
                    strcat(fn, "/status");
                    FILE *f = fopen(fn, "r");
                    if (f == NULL)
                    {
                        fprintf(stderr, "%s, invalid input -- PID <%s> is not a running process\n", argv[0], optarg);
                        exit(1);
                    }
                    fclose(f);
		    break;
		} else {
                    fprintf(stderr, "%s: invalid input -- <pid> must be a positive integer\n", argv[0]);
                    exit(1);
		}
	    case 's':  // the state is displayed if -s is an arg
		if (optarg == NULL)   
		{
		    s_flag = 1;
		    break;	
                // handles any other arg that is not '-s-'. Does similar handling in following else ifs.
		} else if (strcmp(optarg, "-"))   
		{
                    fprintf(stderr, "%s: invalid input -- -%c%s\n", argv[0], option, optarg);
                    exit(1);
		} else {
		    break;
		}
            case 'U':   // the user time is displayed if -U is NOT an arg
		if (optarg == NULL)
		{
		    break;
		} else if (strcmp(optarg, "-"))
		{
                    fprintf(stderr, "%s: invalid input -- -%c%s\n", argv[0], option, optarg);
                    exit(1);
		} else {   // '-U-' turns off user time display 
		    U_flag = 0;
		    break;
	        }	    
            case 'S':   // system time is displayed if -S is an arg
		if (optarg == NULL)
		{
		    S_flag = 1;
		    break;
		} else if (strcmp(optarg, "-"))
		{
                    fprintf(stderr, "%s: invalid input -- -%c%s\n", argv[0], option, optarg);
                    exit(1);
		} else {   
		    break;
	        }	    
	    case 'v':   // displays vmem if -v is an arg
		if (optarg == NULL)
		{
		    v_flag = 1;
		    break;
		} else if (strcmp(optarg, "-"))
		{
                    fprintf(stderr, "%s: invalid input -- -%c%s\n", argv[0], option, optarg);
                    exit(1);
		} else {
		    break;
	        }	    
	    case 'c':   // displays command line that started the prgm if -c is NOT an arg
		if (optarg == NULL)
	        {
	            break;
		} else if (strcmp(optarg, "-"))
		{
                    fprintf(stderr, "%s: invalid input -- -%c%s\n", argv[0], option, optarg);
                    exit(1);
		} else {   // '-c-' will make the command line that started the prgm display
		    c_flag = 0;
		    break;
	        }	    
	    default:
		exit(1);
        }
    }
}
