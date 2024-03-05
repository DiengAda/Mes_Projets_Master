#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>

#define CHEMIN_MAX 128
#define	CHK(op)		do { if ((op) == -1) raler(1, #op); } while (0)

void raler(int syserr, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    if (syserr)
        perror("");
    exit(1);
}

void rotate(int n, char *chemin_fichier_src)
{
    int src, dest;
    char *buffer1, *buffer2;
    char chemin_dest[CHEMIN_MAX]; 
    ssize_t len;
    struct stat st;

    if (strlen(chemin_fichier_src) + 4 > CHEMIN_MAX) // +4 pour .rot
    {
        raler(0, "Longueur du chemin trop grande");
    }

    //le chemin du fichier de destination en ajoutant .rot
    snprintf(chemin_dest, CHEMIN_MAX, "%s.rot", chemin_fichier_src);
  
    CHK(src = open(chemin_fichier_src, O_RDONLY));

    CHK(fstat(src, &st));

    buffer1 = malloc(n);
    buffer2 = malloc(st.st_size - n);

    if (buffer1 == NULL || buffer2 == NULL)
    {
        raler(1, "malloc");
    }

    // On lit les n premiers octets dans buffer1
    CHK(len = read(src, buffer1, n));

    // On lit ensuite le reste dans buffer2
    CHK(len = read(src, buffer2, st.st_size - n));

    CHK(close(src));

    CHK(dest = open(chemin_dest, O_WRONLY | O_CREAT | O_TRUNC, 0666));

    // On écrit  d'abord le contenu de buffer2 dans le fichier dest
    CHK(write(dest, buffer2, len));

    // Et ensuite les n premiers octets contenus dans buffer1
    // dans le fichier dest
    CHK(write(dest, buffer1, n));

    CHK(close(dest));

    free(buffer1);
    free(buffer2);
}

int main(int argc, char *argv[])
{
    int n, i;

    if (argc < 2)
    {
        raler(0, "usage: rotation n f1 ... fn");
    }

    n = atoi(argv[1]);

    if (n == -1)
    {
        raler(0, "Argument n invalide");
    }

    for (i = 2; i < argc; i++)
    {
        rotate(n, argv[i]);
    }

    return 0;
}