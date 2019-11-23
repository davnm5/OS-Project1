#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../include/csapp.h"

void shell(char *arg)
{
    for (;;)
    {
        char input[PRMTSIZE + 1] = {0x0};
        char *ptr = input;
        char *args[MAXARGS + 1] = {NULL};
        int wstatus;
        char wd[256];

        printf("SO<%s>sh:%s%s", arg, getcwd(wd, sizeof(wd)), "$"); //prompt, muestra el directorio actual haciendo uso de la llamada al sistema getcwd(a,b)
        fgets(input, PRMTSIZE, stdin);                             //la entrada estandar se alamacena en el array input;

        // el puntero ignora las entradas con espacios en blanco,saltos de linea y tabs
        if (*ptr == '\n' || *ptr == ' ' || *ptr == '\t')
            continue;

        // se convierte cada linea ingresada a una lista de argumentos
        for (unsigned int i = 0; i < sizeof(args) && *ptr; ptr++)
        {
            if (*ptr == ' ' || *ptr == '\t')
                continue;
            if (*ptr == '\n')
                break;
            for (args[i++] = ptr; *ptr && *ptr != ' ' && *ptr != '\n' && *ptr != '\t'; ptr++)
                ;
            *ptr = '\0';
        }

        if (strcmp(EXIT, args[0]) == 0) //si el comando es exit se invoca a la llamada del sistema exit()
            exit(0);

        if (strcmp(CD, args[0]) == 0) //si el comando es cd se invoca a la llamada del sistema chdir(directorio)
        {
            if (chdir(args[1]) == -1)
                perror("chdir");
        }

        if (fork() == 0)
            exit(execvp(args[0], args)); //el proceso hijo ejecuta el programa recibido
        signal(SIGINT, SIG_IGN);         //ignora la señal de interrupción

        // espera a que el programa finalice
        wait(&wstatus);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso:: ./shell <student_id>\n");
        return 0;
    }
    else
        shell(argv[1]);
}