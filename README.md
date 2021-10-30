# RUNNING THE SHELL-
1.  Compile the code with the make command in the directory containing the code
2.  Run the executable with `./shellex` command 

# FEATURES-

1.  Different commands can be entered in the same prompt separated by ';'
2.  Tabs and extra spaces are removed for all functions
3.  Commands followed by '&' are run in the background while shell continues. It's code is stored in bg_fg.c
4.  Commands pipelines are implemented i.e. commands seperated by '|' have their inputs and outputs redirected
5.  Input/Output redirection(code stored in IOredirection.c) is implemented with the following symbols:
    - '<': Input for the command is taken from the filename specified after '<'
    - '>': Input for the command is overwritten into the filename specified after '>'
    - '>>': Input for the command is appended into the filename specified after '>>'
6. Ctrl z(code stored in ctrl.c) pushes any currently running foreground job into the background, and changes its state from running to stopped. 
7. Ctrl c(code stored in ctrl.c) interrupts any currently running foreground job
8. Ctrl d logs the user out of the shell.
9. All errors like invalid command syntax, system errors, etc. are handled.

# COMMANDS-
The functional commands are as follows:

1. ***cd***: 
    - cd followed by directory path will change the working directory
    - code is stored in commands.c

2. ***echo***: 
    - echo followed by string will print the string
    - code is stored in commands.c

3. ***pwd***: 
    - pwd prints current working directory 
    - code is stored in commands.c

4. ***ls***
    - ls followed by any combination of the flags (-l, -a, -la, -al) and the directories/files
    - The flags -l will list complete information about each directory/file and -a lists all files (including hidden files) 
    - code is stored in ls.c

5. ***pinfo***: 
    - pinfo prints process information about the shell 
    - pinfo followed by process ID prints information about that particular process 
    - Stored in pinfo.c

6. ***execvp***:
    - All non built-in functions are implemented using exevp in bg_fg.c

7. ***repeat***:
    - repeat followed by a whole number and the command to be executed
    - code is stored in commands.c

8. ***jobs***:
    - jobs prints a list of all currently running background processes
    - The flag -r lists all running bg processes and the flag -s lists all stopped bg processes
    - code is stored in processes.c

9. ***sig***:
    - sig sends a signal to a process that's specified by its job number.
    - syntax- `sig <jobnumber> <signal>`
    - code is stored in processes.c

10. ***fg***:
    - fg rings a background process specified by its job number to the foreground and changes its state to running.
    - syntax- `fg <jobnumber>`
    - code is stored in processes.c

11. ***bg***:
    - bg changes the state of a stopped background job to running
    - syntax- `bg <jobnumber>`
    - code is stored in processes.c

12. ***replay***:
    - replay command executes a particular command in fixed time interval for a certain period
    - syntax- `replay -command <command> -interval <number> -period <number>`
    - code is stored in replay.c



# ASSUMPTIONS:

- Directory names are at most 1000 characters long
- Input on the shell are at most 2000 characters long
- At most 50 processes can be stored as background processes
- At most 30 directories can be passed to the ls commands
Note: All these are arbitrary limits and can be modified by modifying code