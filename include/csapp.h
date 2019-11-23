#ifndef _CSAPP_H
#define _CSAPP_H

#define PRMTSIZE 255
#define MAXARGS 63
#define EXIT "exit"
#define CD "cd"
#define NTHREADS 4
/*
Estructura asignada a cada uno de los hilos, indican el inicio y fin de cada bloque
*/
typedef struct estructuraTDA
{
    int inicio;
    int fin;
} estructura;

#endif