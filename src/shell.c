#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "../include/csapp.h"

void shell(char *arg)
{
    while (1) //ciclo indefinido hasta que se ejecute exit().
    {
        char input[SIZE] = {0x0}; //array para la entrada
        char *ptr = input;
        char *args[MAXARGS] = {NULL}; //array para los argumentos de los comandos
        int wstatus;
        char wd[SIZE] = {0x0};

        printf("SO<%s>sh:%s%s", arg, getcwd(wd, sizeof(wd)), "$"); //prompt, muestra el directorio actual haciendo uso de la llamada al sistema getcwd(a,b)
        fgets(input, SIZE, stdin);                                 //la entrada estándar se almacena en el array input

        // el puntero ignora las entradas con espacios en blanco,saltos de linea y tabs
        if (*ptr == '\n' || *ptr == ' ' || *ptr == '\t')
            continue;

        // se convierte cada linea ingresada a una lista de argumentos
        for (unsigned int i = 0; i < sizeof(args) && *ptr; ptr++)
        {
            if (*ptr == ' ' || *ptr == '\t')
                continue;
            if (*ptr == '\n') //cuando hay un salto de línea se sale del ciclo y hasta ahí se llenará la lista de argumentos.
                break;
            for (args[i++] = ptr; *ptr && *ptr != ' ' && *ptr != '\n' && *ptr != '\t'; ptr++)
                ;
            *ptr = '\0';
        }

        if (strcmp(EXIT, args[0]) == 0) //si el comando es exit se invoca a la llamada del sistema exit()
            exit(0);

        if (strcmp(CD, args[0]) == 0) //si el comando es cd se invoca a la llamada del sistema chdir(directorio)
        {
            if(args[1]==NULL){ //si el comando cd no especifíca el directorio, entonces se lo lleva al directorio raíz.
                chdir("/");
            }
            
            else if (chdir(args[1]) == -1) //si el directorio no existe se muestra el mensaje de error
                perror("Error");
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
    if (argc != 2) //si no hay dos argumentos 
    {
        printf("Uso:: ./shell <student_id>\n");
        return 0;
    }
    else
        shell(argv[1]); //al método shell se le envía el id del estudiante.
}