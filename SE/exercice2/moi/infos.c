#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <stdnoreturn.h>
#include <fcntl.h>
#include <ctype.h>


#define MAXBUF 4096
#define CHEMIN_MAX 128

#define CHK(op)     do { if ((op) == -1) raler(1, #op); } while (0)
#define CHKN(op)    do { if ((op) == NULL) raler(1, #op); } while (0)

noreturn void raler(int syserr, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    if(syserr)
        perror ("");
    exit (1);
}

typedef struct  {
    ino_t inode;
    off_t taille;
    off_t nbLignes;
    off_t nbLettres;
    char chemin[CHEMIN_MAX + 1];
} infos_fichier;

//un tableau de pointeurs sur struct pour stocker les infos sur les fichiers
infos_fichier *TableauInfos;
size_t infos_len = 0;

// fonction pour recuperer les infos sur les fichiers
void recuprer_infos(char *fichier, int *lignes, int *lettres)
{
    int fd, n, i, nbLignes = 0, nbLettres = 0;
    char buf[MAXBUF];

    CHK(fd = open(fichier, O_RDONLY));

    while ((n = read(fd, buf, MAXBUF)) > 0)
    {
        for (i = 0; i < n; i++)
        {
            if (buf[i] == '\n')
                nbLignes++;
            if (isalpha(buf[i]))
                nbLettres++;
        }
    }

    CHK(n);
    *lignes = nbLignes;
    *lettres = nbLettres;

    CHK(close(fd));
}

// fonction pour ajouter les infos sur les fichiers
void ajouter_info(ino_t inode, off_t taille, int ligne,
                    int lettre, char *chmn)
{
    CHKN(TableauInfos = realloc(TableauInfos,
                            (infos_len + 1) * sizeof(infos_fichier)));
    // Ajout des infos
    TableauInfos[infos_len].inode = inode;
    TableauInfos[infos_len].taille = taille;
    TableauInfos[infos_len].nbLignes = ligne;
    TableauInfos[infos_len].nbLettres = lettre;
    strncpy(TableauInfos[infos_len].chemin, chmn, CHEMIN_MAX + 1);
    infos_len++;
    
}

// fonction pour parcourir le repertoire
void parcourir(char *repertoire)
{
    DIR *dir;
    struct dirent *entree;
    char nouveau_chemin[CHEMIN_MAX + 1];
    int lignes, lettres, n;
    struct stat st;

    CHKN(dir = opendir(repertoire));

    errno = 0;
    while ((entree = readdir(dir)) != NULL)
    {
        if (strcmp(entree->d_name, ".") != 0 
                    && strcmp(entree->d_name, "..") != 0)
        {
            // on construit le chemin complet
            n = snprintf(nouveau_chemin, sizeof nouveau_chemin, "%s/%s",
                            repertoire, entree->d_name);

            if (n < 0 || n > CHEMIN_MAX) 
                raler(0, "chemin '%s/%s' trop long",repertoire,
                            entree->d_name);

            // on recupere les infos sur le fichier
            CHK(lstat(nouveau_chemin, &st));

            // on traite selon c'est un fichier regulier ou non
            switch (st.st_mode & S_IFMT)
            {
                case S_IFDIR: //si repertoire on parcourt recursivement
                    parcourir(nouveau_chemin);
                    break;
                case S_IFREG: //si fichier regulier

                    // on recupere les infos sur le fichier
                    recuprer_infos(nouveau_chemin, &lignes, &lettres);
                    
                    // on les ajoute dans le tableau
                    ajouter_info(st.st_ino, st.st_size, lignes,
                                        lettres, nouveau_chemin);

                    break;
                case S_IFLNK: //si lien symbolique on ignore
                    break;
                default: // autre type de fichier on ignore aussi
                    break;
            }
        }
        errno = 0;
    }
    if (errno != 0)
        raler(0, "readdir");
    CHK(closedir(dir));
}

// fonction pour comparer les inodes
int compare(const void *a, const void *b)
{
    return ((infos_fichier *)a)->inode - ((infos_fichier *)b)->inode;
}

// fonction pour afficher les infos
void afficher(infos_fichier *TableauInfos, size_t infos_len)
{
    size_t i;
    for (i = 0; i < infos_len; i++)
    {
        printf("%ld %ld %ld %ld %s\n", TableauInfos[i].inode,
                TableauInfos[i].taille, TableauInfos[i].nbLignes,
                TableauInfos[i].nbLettres, TableauInfos[i].chemin);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage : infos repertoire\n");
        exit(1);
    }

    // on parcourt le repertoire passé en argument
    parcourir(argv[1]);

    // on trie la sortie par inode
    qsort(TableauInfos, infos_len, sizeof(infos_fichier), compare);

    // on affiche les infos
    afficher(TableauInfos, infos_len);

    // on libere la memoire
    free(TableauInfos);

    exit(0);   
}