#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#define MAX 1000000

char buffer[MAX];
int bytes_read;
unsigned lines = 1;
unsigned words = 1;

int main(int argc, char *argv[])
{

    int fd = open("file2.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("r1");
        exit(1);
    }

    else
    {
        while (bytes_read = read(fd, buffer, sizeof(buffer)))
        {

            for (int i = 0; i < bytes_read; i++)
            {
                if (buffer[i] == '\n')
                {
                    if (i > 1 && buffer[i - 2] != '\n')
                    {
                        ++words;
                    }
                    ++lines;
                }

                if (buffer[i] == ' ')
                {
                    ++words;
                }
            }
        }
        printf("%d\n", words);
        printf("%d\n", lines);
        return 0;
    }
}
