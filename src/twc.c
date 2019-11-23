#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "../include/csapp.h"

unsigned char *buffer;
size_t bloque;
sem_t sem1, sem2;
unsigned int lines = 0;
unsigned int words = 1;

void *contador(void *arg)
{
    estructura *argumento = (estructura *)arg;
    for (int i = argumento->inicio; i <= argumento->fin; i++)
    {
        if (buffer[i] == '\n')
        {
            if (i > 1 && (buffer[i - 2] != '\n'))
            {
                sem_wait(&sem2);
                ++words;
                sem_post(&sem2);
            }
            sem_wait(&sem1);
            ++lines;
            sem_post(&sem1);
        }

        if (buffer[i] == ' ')
        {
            sem_wait(&sem2);
            ++words;
            sem_post(&sem2);
        }
    }
    return (void *)0;
}

void dividir()
{
    unsigned int inicio = 0;
    unsigned int fin = bloque;
    pthread_t *hilos = malloc(NTHREADS * sizeof(pthread_t));
    for (int i = 0; i < NTHREADS; i++)
    {
        estructura *argumento = malloc(sizeof(estructura));
        argumento->inicio = inicio;
        argumento->fin = fin;
        int status = pthread_create(&hilos[i], NULL, contador, (void *)argumento);
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
        int status = pthread_join(hilos[j], NULL);
        if (status < 0)
        {
            fprintf(stderr, "Error al esperar por el hilo %d \n", j);
            exit(1);
        }
    }
}

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
        int fd = open(argv[2], O_RDONLY);
        if (fd < 0)
        {
            fprintf(stderr, "Error: el archivo %s no existe\n", argv[2]);
            exit(1);
        }

        else
        {
            sem_init(&sem1, 0, 1);
            sem_init(&sem2, 0, 1);
            size_t size = lseek(fd, 0, SEEK_END);
            if (size % 4 != 0)
                size = size + 1;
            buffer = (unsigned char *)calloc(size, sizeof(unsigned char));
            lseek(fd, 0, SEEK_SET);
            read(fd, buffer, size);
            bloque = size / 4;
            dividir();
            imprimir(argv[1]);
        }
    }
    else
    {
        printf("Uso:: ./twc [OPCION] [ARCHIVO]\n [OPCIONES]:\n -l: numero de lineas\n -w: numero de palabras\n");
    }

    return 0;
}
