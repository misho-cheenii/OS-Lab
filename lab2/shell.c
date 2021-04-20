#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80 /* 80 chars per line, per command */
#define DELIMITERS " \t\n"

/*
Function: refresh_args

Free the old content of args and set to NULL to avoid memory leaks
*/

void clear_args(char *args[])
{
    while(*args) {
        free(*args);  // to avoid memory leaks
        *args++ = NULL;
    }
}

/*Function: parse_input

Parse input and store arguments
Saves recent history command
returns the number of arguments
*/

size_t parse_input(char *args[], char *input)
{
    char input_buffer[MAX_LINE + 1];
    size_t num = 0;
    char command[MAX_LINE + 1];

    if(fgets(input_buffer, MAX_LINE + 1, stdin) == NULL)
    {
        fprintf(stderr, "Failed to read input!\n");
    }

    if(strncmp(input_buffer, "!!", 2) == 0) 
    {
        if(strlen(input) == 0) 
        {
            fprintf(stderr, "No history available yet!\n");
        }
        printf("%s", input);
    }
    strcpy(input, input_buffer);    //Replace history with new command

    strcpy(command, input);
    char *token = strtok(command, DELIMITERS);

    while(token != NULL) 
    {
        args[num] = malloc(strlen(token) + 1);
        strcpy(args[num], token);
        ++num;
        token = strtok(NULL, DELIMITERS);
    }

    return num;
}

/*
Function: changeDirectory

Changes directory when cd is called.
cd with no arguments changes to home directory
cd(space) with ".." goes to parrent directory
returns success or not
*/
int changeDirectory(char *args[], int size) {
    int val;
    if(size == 1)
        val = chdir(getenv("HOME"));
    
    else if(strcmp(args[1], "..")==0 )
        val = chdir("..");
    
    else
        val = chdir(args[1]);

    if(val!=0)
        fprintf(stderr, "Invalid directory!\n");

    return val;
}

/*
Function: redirect_io

Check the redirection tokens in arguments and remove such tokens.
IO redirection: bit 1 for output, bit 0 for input)
returns success or not
*/

int redirect_io(char ** args, size_t *size)
{
    int fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    size_t to_remove[4], remove_cnt = 0;

    for(size_t i = 0; i != *size; ++i) 
    {
        if(remove_cnt >= 4) 
        {
            break;
        }

        // input
        if(strcmp("<", args[i]) == 0) 
        {     
            to_remove[remove_cnt++] = i;
            if(i == (*size) - 1) 
            {
                fprintf(stderr, "No input file provided!\n");
                return 0;
            }
            fd = open(args[i+1], O_RDONLY, mode);
            if(fd < 0) 
            {
                fprintf(stderr, "Failed to open the input file: %s\n", args[i+1]);
                return 0;
            }
            dup2(fd, 0);
            to_remove[remove_cnt++] = ++i;
            close(fd);
        } 

        // output
        else if(strcmp(">", args[i]) == 0)
        {   
            to_remove[remove_cnt++] = i;
            if(i == (*size) - 1)
            {
                fprintf(stderr, "No output file provided!\n");
                return 0;
            }
            fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, mode);
            if(fd < 0) 
            {
                fprintf(stderr, "Failed to open the output file: %s\n", args[i+1]);
                return 0;
            }
            dup2(fd, 1);
            to_remove[remove_cnt++] = ++i;
            close(fd);
        }
    }

    for(int i = remove_cnt - 1; i >= 0; --i) 
    {
        size_t pos = to_remove[i];  // the index of arg to remove
        while(pos != *size) 
        {
            args[pos] = NULL;
            ++pos;
        }
        --(*size);
    }

    return 1;
}


/*
Function: run_command

Runs the command written in shell
Returns success or not
*/

int run_command(char **args, size_t args_num) 
{
    /* Create a child process and execute the command */
    pid_t pid = fork();
    if(pid < 0) // Fork failed
    {   
        fprintf(stderr, "Failed to fork!\n");
        return 0;
    } 
    
    else if (pid == 0) // child process
    { 
        /* Redirect I/O */
        if(redirect_io(args, &args_num) == 0)
        {
            return 0; // Redirect failed
        }
        execvp(args[0], args);
        fflush(stdin);
    } 

    else // parent process
    { 
        wait(NULL);
    }

    return 1;
}


int main(void) 
{
    char *args[MAX_LINE / 2 + 1] = { NULL };
    char command[MAX_LINE + 1] = "";
    int should_run = 1; /* flag to determine when to exit program */

    while (should_run) 
    {
        printf("osh>");
        fflush(stdout);
        fflush(stdin);

        /* Make args empty before parsing */
        clear_args(args);

        /* Get input and parse it */
        size_t args_num = parse_input(args, command);

        /* Continue or exit */

	if(args_num != 0) 
	{
	    if(strcmp(args[0], "cd") == 0)
    	    {
	        if(changeDirectory(args, args_num) != 0)
		    continue;
	    }

    	    else if(strcmp(args[0], "exit") == 0)
                should_run = 0;
	    
	    else
		/* Run command */
		run_command(args, args_num);
	}

	else // empty input
	{ 
            printf("Please enter the command! (or type \"exit\" to exit)\n");
            continue;
        }
        
    }
    
    clear_args(args);
    return 0;
}
