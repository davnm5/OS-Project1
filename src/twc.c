#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "../include/csapp.h"

unsigned char *buffer; //buffer para el archivo.
size_t bloque; // variable que almacena el tamaño en bytes del bloque.
sem_t sem1, sem2; //semáforos.
unsigned int lines = 0; //contador de líneas.
unsigned int words = 1; //contador de palabras.

/*
Método al que acceden los hilos de forma sincronizada, se realiza el conteo de palabras y líneas.
*/
void *contador(void *arg)
{
    estructura *argumento = (estructura *)arg; //se le asigna a la estructura local el argumento recibido que es de tipo estructura.
    for (int i = argumento->inicio; i <= argumento->fin; i++)
    {
        if (buffer[i] == '\n') //ingresa si se trata de una nueva linea.
        {
            if (i > 1 && (buffer[i - 2] != '\n')) //condición que verifica si antes del salto de linea hay un caracter, entonces hay una palabra. 
            {
                sem_wait(&sem2); 
                ++words;          //región crítica
                sem_post(&sem2);
            }
            sem_wait(&sem1);
            ++lines;             //región crítica
            sem_post(&sem1);
        }

        if (buffer[i] == ' ') //ingresa si se trata de una nueva palabra.
        {
            sem_wait(&sem2);
            ++words;          //región crítica
            sem_post(&sem2);
        }
    }
    free(argumento);// se libera la memoria asiganada a la estructura
    return (void *)0;
}

/*
Método que se encarga de asignar a cada estructura de los hilos un inicio y fin del bloque a procesar.
*/
void dividir()
{
    unsigned int inicio = 0;
    unsigned int fin = bloque;
    pthread_t *hilos = malloc(NTHREADS * sizeof(pthread_t));// reserva de memoria para los hilos
    for (int i = 0; i < NTHREADS; i++)
    {
        estructura *argumento = malloc(sizeof(estructura)); // reserva de memoria para las estructuras
        argumento->inicio = inicio;
        argumento->fin = fin;
        int status = pthread_create(&hilos[i], NULL, contador, (void *)argumento); //creación de hilos
        if (status < 0)
        {
            fprintf(stderr, "Error al crear el hilo: %d \n", i);
            exit(1);
        }

        inicio = fin + 1;
        fin = fin + bloque;
    }

    for (int j = 0; j < NTHREADS; j++)
    {
        int status = pthread_join(hilos[j], NULL); //esperando a los hilos
        if (status < 0)
        {
            fprintf(stderr, "Error al esperar por el hilo %d \n", j);
            exit(1);
        }
    }

    free(hilos); //se libera la memoria asignada a los hilos
}
/*Método que se encarga de imprimir las palabras y lineas totales, dependiendo del parámetro elegido*/
void imprimir(char *parametro)
{

    if (strcmp(parametro, "-l") == 0)
    {
        printf("Lineas totales: %d\n", lines);
    }

    else
    {
        printf("Palabras totales: %d\n", words);
    }
}

int main(int argc, char *argv[])
{
    if (argc == 3 && (strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "-w") == 0))
    {
        int fd = open(argv[2], O_RDONLY); //llamada al sistema que permite abrir un archivo
        if (fd < 0)
        {
            fprintf(stderr, "Error: el archivo %s no existe\n", argv[2]);
            exit(1);
        }

        else
        {
            sem_init(&sem1, 0, 1); //se inicializa el semáforo para el contador de líneas
            sem_init(&sem2, 0, 1); //se inicializa el semáforo para el contador de palabras
            size_t size = lseek(fd, 0, SEEK_END); //llamada al sistema que devuelve el tamaño del archivo en bytes desde el inicio hasta el fin.
            if (size % 4 != 0)
                size = size + 1;
            buffer = (unsigned char *)calloc(size, sizeof(unsigned char)); //se asigna la cantidad justa de memoria para el archivo,los bloques se inicializan con cero.
            lseek(fd, 0, SEEK_SET); //llamada al sistema que ubica el desplazamiento del archivo en el inicio.
            read(fd, buffer, size); //llamada al sistema que permite leer desde un descriptor de archivo.
            bloque = size / 4;
            dividir();
            imprimir(argv[1]);
            free(buffer); //se libera la memoria asignada al buffer
        }
    }
    else
    {
        printf("Uso:: ./twc [OPCION] [ARCHIVO]\n [OPCIONES]:\n -l: numero de lineas\n -w: numero de palabras\n");
    }

    return 0;
}
