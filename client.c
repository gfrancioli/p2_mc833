#include "params.h"

void print_menu()
{
    printf("\n/===============================/\n\n");
    printf("1 - Cadastro\n");
    printf("2 - Listar todos os filmes\n");
    printf("3 - Listar por titulos\n");
    printf("4 - Listar por ID\n");
    printf("5 - Listar por genero\n");
    printf("6 - Remover um filme\n");
    printf("7 - Adicionar um genero a um filme\n");
    printf("\n/===============================/\n\n");
}

// parse to text the inputs with separator
void parseString(movie movie, char *result)
{
    char aux[7];
    strcat(result, movie.titulo);
    strcat(result, SPECIALSEP);
    strcat(result, movie.genero);
    strcat(result, SPECIALSEP);
    strcat(result, movie.diretor);
    strcat(result, SPECIALSEP);
    strcat(result, movie.ano);
    strcat(result, SPECIALSEP);
}

// receive messages with timeout
int receive_timeout(int sock, char *buff, int *length, struct sockaddr_in *connection, int sectimeout)
{
    fd_set masterfds;
    struct timeval time;
    FD_ZERO(&masterfds);
    FD_SET(sock, &masterfds);
    time.tv_sec = sectimeout;
    if (select(sock + 1, &masterfds, NULL, NULL, &time) && recvfrom(sock, buff, *length, 0, (struct sockaddr *)connection, (socklen_t *)length) != -1)
        return 1;
    else
        return 0;
}

// catch all input of register
void cadastro(movie *novo_filme)
{
    char txt[1000];
    movie aux_movie;
    for (;;)
    {
        printf("Titulo: \n");
        fgets(txt, MAX, stdin);
        strtok(txt, "\n");
        if (strlen(txt) > 0 && strcmp(txt, "\n"))
        {
            strcpy(aux_movie.titulo, txt);
            break;
        }
        else
        {
            printf("Campo não pode ser vazio.\n");
        }
    }
    memset(txt, 0, sizeof(txt));

    for (;;)
    {
        printf("Genero: \n");
        fgets(txt, MAX, stdin);
        strtok(txt, "\n");
        if (strlen(txt) > 0 && strcmp(txt, "\n"))
        {
            strcpy(aux_movie.genero, txt);
            break;
        }
        else
        {
            printf("Campo não pode ser vazio.\n");
        }
    }
    memset(txt, 0, sizeof(txt));

    for (;;)
    {
        printf("Diretor: \n");
        fgets(txt, MAX, stdin);
        strtok(txt, "\n");
        if (strlen(txt) > 0 && strcmp(txt, "\n"))
        {
            strcpy(aux_movie.diretor, txt);
            break;
        }
        else
        {
            printf("Campo não pode ser vazio.\n");
        }
    }
    memset(txt, 0, sizeof(txt));

    for (;;)
    {
        printf("Ano: \n");
        fgets(txt, MAX, stdin);
        strtok(txt, "\n");
        if (strlen(txt) > 0 && strcmp(txt, "\n"))
        {
            strcpy(aux_movie.ano, txt);
            break;
        }
        else
        {
            printf("Campo não pode ser vazio.\n");
        }
    }
    memset(txt, 0, sizeof(txt));
    *novo_filme = aux_movie;
}

void func(int sockfd, struct sockaddr_in *servaddr)
{
    char buff[MAX], serveres[2000];
    int retorno = 0, n = 0;
    bzero(buff, sizeof(buff));
    movie novo_filme;

    for (;;)
    {

        print_menu();
        fgets(buff, 2000, stdin);
        strtok(buff, "\n");
        
        // make register in client side
        if (!strcmp(buff, CADASTRO))
        {
            strcat(buff, SPECIALSEP);
            cadastro(&novo_filme);
            parseString(novo_filme, buff);
            while (1)
            {
                sendto(sockfd, buff, strlen(buff), 0, (const struct sockaddr *)servaddr, sizeof(*servaddr));
                bzero(serveres, sizeof(serveres));
                n = sizeof(serveres);
                retorno = receive_timeout(sockfd, serveres, &n, servaddr, 3);
                if (retorno == 1)
                {
                    break;
                }
            }
            printf("%s\n", serveres);
        }
        else
        {
            // send command to server
            for (;;)
            {
                sendto(sockfd, buff, strlen(buff), 0, (const struct sockaddr *)servaddr, sizeof(*servaddr));
                bzero(serveres, sizeof(serveres));
                n = sizeof(serveres);
                retorno = receive_timeout(sockfd, serveres, &n, servaddr, 10);
                if (retorno == 1)
                {
                    break;
                }
            }
            printf("Resultados:\n", serveres);
            printf("%s\n", serveres);
        }
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd != -1)
        printf("Socket criado com sucesso!\n");
    else {
        printf("Falha na criacao do socket\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

	// function for messages
    func(sockfd, &servaddr);
    // close socket
    close(sockfd);
}