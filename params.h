#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 2000
#define PORT 3000
#define LISTA_TITULOS 3
#define LISTA_IDS 4
#define LISTA_GENEROS 5

static const char SPECIALSEP[] = "-";
static const char CADASTRO[MAX] = "1";
static const char LISTA_TODOS[MAX] = "2";
static const char LISTA_TITULO[MAX] = "3";
static const char LISTA_ID[MAX] = "4";
static const char LISTA_GENERO[MAX] = "5";
static const char DELETE[MAX] = "6";
static const char GENERO[MAX] = "7";

typedef struct _movie {
    char id[MAX];
    char titulo[MAX];
    char genero[MAX];
    char diretor[MAX];
    char ano[MAX];
    char deleted[MAX];
} movie;