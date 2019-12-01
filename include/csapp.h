#ifndef _CSAPP_H
#define _CSAPP_H

#define SIZE 256
#define MAXARGS 64
#define EXIT "exit"
#define CD "cd"
/*
Estructura asignada a cada uno de los hilos, indican el inicio y fin de cada bloque
*/
typedef struct estructuraTDA
{
    int inicio;
    int fin;
} estructura;

#endif