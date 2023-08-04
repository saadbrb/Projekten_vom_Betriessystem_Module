#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <algorithm>

#include <sys/types.h>
#include <sys/wait.h>


#define TOKENSIZE 100

void  StrTokenizer(char *line, char **argv);
void  myExecvp(char **argv);
void CtrlCSignalHandle(int);
void ChildSignalHandle(int);
static volatile int keepRunning = 1;
auto begin = std::chrono::high_resolution_clock::now();

pid_t  pid;

char specificElement;

bool background = false;

int main()
{
        char *path2;
        char *arr [250];
        char *Tokenized ;
        char input[250];
        char *argv[TOKENSIZE];


        signal(SIGINT, CtrlCSignalHandle);
        signal(SIGCHLD, ChildSignalHandle);

        while(keepRunning)
        {
            std::cout<< "myShell > ";
            std::cin.getline(input,250);

            StrTokenizer(input, argv);
            if (strcmp(input, "\n") == 0){
                continue;
            }
            else if (strcmp(input, "exit") == 0)
            {
                char c_input;
                std::cout << "Are you sure to exit shell?  y/n" <<std::endl;
                std::cin >> c_input;
                if (c_input == 'y') {
                    exit(0);
                }
                if (c_input == 'n') {
                    continue;
                }
            }else if (strcmp(argv[0], "cd") == 0)
        {
            if (argv[1] != NULL)
            {
                if (chdir(argv[1]) != 0)
                {
                    std::cout << "Error: Failed to change directory" << std::endl;
                }
            }
            else
            {
                std::cout << "Error: Missing argument for cd command" << std::endl;
            }
        }
            else{
                myExecvp(argv);
            }
        }

    return 0;
}
void  myExecvp(char **argv)
{
    int status;
    int childStatus;

    pid = fork();
    if(pid == 0)
    {
        childStatus = execvp(*argv, argv);
        if (childStatus < 0){
            std::cout<<"ERROR:wrong input"<<std::endl;
        }
        exit(1);        //should be: exit(childStatus) better

    }
    else if(pid < 0)
    {
        std::cout<< "somthing went wrong!"<<std::endl;
    }

    else if (background == true) {
        std:: cout << pid << std::endl;
        background = false;

    }

    else
    {
        int status;
        waitpid(pid, &status , WUNTRACED|WCONTINUED);
    }
}

void StrTokenizer(char *input, char **argv)
{

    if (input[strlen(input)-1] == '&') {
            input[strlen(input)-1] = '\0';
            background = true;
    }


    char *stringTokenized;
    stringTokenized = strtok(input, " ");
    while(stringTokenized != NULL)
    {
        *argv++  = stringTokenized;
        stringTokenized = strtok(NULL, " ");
    }

    *argv = NULL;
}
void ChildSignalHandle(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
void CtrlCSignalHandle(int sig) {
    keepRunning = 0;
    signal(sig, SIG_IGN);
    auto end = std::chrono::high_resolution_clock::now();
       auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            std::cout << '\n';
            std::cout << "stop" <<std::endl;
            printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
            exit(1);
}
