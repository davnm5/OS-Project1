#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PRMTSIZ 255
#define MAXARGS 63
#define EXIT "exit"
#define CD "cd"

void shell(char *arg)
{
    for (;;)
    {
        char input[PRMTSIZ + 1] = {0x0};
        char *ptr = input;
        char *args[MAXARGS + 1] = {NULL};
        int wstatus;
        char wd[256];

        // prompt
        printf("SO<%s>sh:%s%s", arg, getcwd(wd, sizeof(wd)), "$");
        fgets(input, PRMTSIZ, stdin);

        // ignore empty input
        if (*ptr == '\n' || *ptr == ' ')
            continue;

        // convert input line to list of arguments
        for (int i = 0; i < sizeof(args) && *ptr; ptr++)
        {
            if (*ptr == ' ')
                continue;
            if (*ptr == '\n')
                break;
            for (args[i++] = ptr; *ptr && *ptr != ' ' && *ptr != '\n'; ptr++);
            *ptr = '\0';
        }

        // built-in: exit
        if (strcmp(EXIT, args[0]) == 0)
            exit(0);
        // built-in: cd
        if (strcmp(CD, args[0]) == 0)
        {
            if (chdir(args[1]) == -1)
                perror("chdir");
        }

        // fork child and execute program
        signal(SIGINT, SIG_DFL);
        if (fork() == 0)
            exit(execvp(args[0], args));
        signal(SIGINT, SIG_IGN);

        // wait for program to finish
        wait(&wstatus);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:: ./shell <student_id>\n");
        return 0;
    }
    else shell(argv[1]);
}