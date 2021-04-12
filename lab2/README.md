# Project 1—UNIX Shell

This project consists of designing a C program to serve as a shell interface
that accepts user commands and then executes each command in a separate
process. Your implementation will support input and output redirection.
Completing this project will involve using the UNIX fork(), exec(), wait()
and dup2() system calls and can be completed on any Linux, UNIX, or macOS
system.

### This project is organized into several parts:
1. Creating the child process and executing the command in the child
2. Providing a history feature
3. Adding support of input and output redirection

### How to use:
- Run **gcc shell.c -o shell** on terminal.
- For history enter **!!**.
- For I/O redirection use the **>** and **<** redirection operators, where ‘>’ redirects the output of a command to a file and ‘<’ redirects
the input to a command from a file. For example, if a user enters

<p align="center">
    osh>ls > out.txt
</p>

the output from the ls command will be redirected to the file out.txt. Similarly, input can be redirected as well. For example, if the user enters

<p align="center">
    osh>sort < in.txt
</p>

the file in.txt will serve as input to the sort command.
