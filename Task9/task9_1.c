#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int main (void) 
{
    char str[] = "String from file";

    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd < 0) 
    {
        perror("Error opening file for writing!\n");
        exit(EXIT_FAILURE);
    }

    int writtenBytes = write(fd, str, strlen(str));

    if (writtenBytes < 0) 
    {
        perror("Error writing to file!\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);

    fd = open("output.txt", O_RDONLY);

    if (fd < 0) 
    {
        perror("Error opening file for reading!\n");
        exit(EXIT_FAILURE);
    }

    printf("Reversed string from file output.txt: ");
    off_t position = lseek(fd, 0, SEEK_END);
    char c;
    if (position < 0) 
    {
        perror("Error position in file!\n");
        close(fd);
        exit(EXIT_FAILURE);
    }
    int readBytes = read(fd, &c, 1);

    if (readBytes < 0) 
    {
        perror("Error reading from file!\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("%c", c);

    while (position > 0) 
    {
        position--;

        position = lseek(fd, position, SEEK_SET);
        if (position < 0) 
        {
            perror("Error position in file!\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        readBytes = read(fd, &c, 1);
        if (readBytes < 0) 
        {
            perror("Error reading from file!\n");
            close(fd);
            exit(EXIT_FAILURE);
        }

        printf("%c", c);
    }

    printf("\n");
    close(fd);

    return 0;
}