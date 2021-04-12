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
⋅⋅⋅osh>ls > out.txt
⋅⋅⋅the output from the ls command will be redirected to the file out.txt. Similarly, input can be redirected as well. For example, if the user enters
⋅⋅⋅osh>sort < in.txt
⋅⋅⋅the file in.txt will serve as input to the sort command.
1. Actual numbers don't matter, just that it's a number
⋅⋅1. Ordered sub-list
4. And another item.

⋅⋅⋅You can have properly indented paragraphs within list items. Notice the blank line above, and the leading spaces (at least one, but we'll use three here to also align the raw Markdown).

⋅⋅⋅To have a line break without a paragraph, you will need to use two trailing spaces.⋅⋅
⋅⋅⋅Note that this line is separate, but within the same paragraph.⋅⋅
⋅⋅⋅(This is contrary to the typical GFM line break behaviour, where trailing spaces are not required.)
