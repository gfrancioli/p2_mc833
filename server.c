#include "params.h"

static const char MOVIE_FILE[] = "movie.txt";
static const char TEMP[] = "temp.txt";

void *mid_controller(int fd, char *msg, struct sockaddr_in);
void controller(char *msg, char *result);
void cadastra(char *buff, char *aux_msg);
void remove_movie(char* buff, char *genero);
void listAll(char *result);
void list(char *result, char *filtro, int opt);
char* countId();

// add type
void adiciona_genero(char* buff, char *split){
	FILE *ptr, *temp;
    movie read_movie;
	int flag = 0;
    char aux[MAX];
    strcpy(aux, split);
    char *id = strtok(aux, SPECIALSEP);
    char *genero = strtok(NULL, "");
	ptr = fopen(MOVIE_FILE, "r");
	temp = fopen("temp.txt", "w");

	//find movie, add type and copy others to temp file
	while (fread(&read_movie, sizeof(movie), 1, ptr)){
		if (!strcmp(read_movie.id, id)){

			strcat(read_movie.genero, ",");
			strcat(read_movie.genero, genero);
			fwrite(&read_movie, sizeof(read_movie), 1, temp);
			flag++;
		}
		else
			fwrite(&read_movie, sizeof(read_movie), 1, temp);
	}

	//remove older file and rename temp file
	fclose(ptr);
	fclose(temp);
	remove(MOVIE_FILE);
	rename("temp.txt", MOVIE_FILE);
	if (flag){
		strcat(buff, "Filme atualizado!\n");
		flag = 0;
	}
	else
		strcat(buff, "Nao foi encontrado nenhum filme com esse ID\n");

}

// register new movie
void cadastra(char *buff, char *aux_msg)
{
    FILE *movie_file;
    char msg[MAX], count[MAX];
    movie new_movie, aux;
    char *dado = strtok(aux_msg, SPECIALSEP);
    movie_file = fopen(MOVIE_FILE, "a");

    // id count
    countId(count);

    // copy message to a new movie
    strcpy(new_movie.titulo, strtok(NULL, SPECIALSEP));
    strcpy(new_movie.genero, strtok(NULL, SPECIALSEP));
    strcpy(new_movie.diretor, strtok(NULL, SPECIALSEP));
    strcpy(new_movie.ano, strtok(NULL, SPECIALSEP));
    strcpy(new_movie.deleted, "0");
    strcpy(new_movie.id, count);

    // insert in movie file
    fwrite(&new_movie, sizeof(new_movie), 1, movie_file);
    fclose(movie_file);

    strcpy(buff, "\n\n\nFilme adicionado\n");
}

// remove movie
void remove_movie(char* buff, char *genero){
	FILE *ptr, *temp;
    movie read_movie;
	int flag = 0;
	ptr = fopen(MOVIE_FILE, "r");
	temp = fopen("temp.txt", "w");

	//find movie and copy the others to temp file
	while (fread(&read_movie, sizeof(movie), 1, ptr)){
		if (!strcmp(read_movie.id, genero)){
			flag++;
		}
		else {
			fwrite(&read_movie, sizeof(read_movie), 1, temp);
		}
	}

	//remove older file and rename temp file
	fclose(ptr);
	fclose(temp);
	remove(MOVIE_FILE);
	rename("temp.txt", MOVIE_FILE);

	if (flag){
		strcat(buff, "Filme deletado!\n");
		flag = 0;
	}
	else
		strcat(buff, "Nao foi encontrado nenhum filme com esse ID\n");
}

// count id to insert correctly
char* countId(char *count)
{
    FILE *movie_file;
    movie m;
    strcpy(count, "0");
    int aux;

    movie_file = fopen(MOVIE_FILE, "r");
    if (!movie_file)
        return "1";

    while (fread(&m, sizeof(movie), 1, movie_file))
        strcpy(count, m.id);
    if (strlen(count) == 0)
    {
        fclose(movie_file);
        return "1";
    }
    fclose(movie_file);
    aux = atoi(count);
    aux++;
    sprintf(count, "%d", aux);
    return count;
}

// list all
void listAll(char *lista)
{
    FILE *movie_file;
    movie m;

    movie_file = fopen(MOVIE_FILE, "r");

    while (fread(&m, sizeof(movie), 1, movie_file))
    {
        strcat(lista, "Id -> ");
        strcat(lista, m.id);
        strcat(lista, " | Titulo -> ");
        strcat(lista, m.titulo);
        strcat(lista, " | Genero -> ");
        strcat(lista, m.genero);
        strcat(lista, " | Diretor -> ");
        strcat(lista, m.diretor);
        strcat(lista, " | Ano Lancamento -> ");
        strcat(lista, m.ano);
        strcat(lista, "\n");
    }
    if (!strcmp(lista, ""))
        strcat(lista, "Nenhum movie cadastrado.\n");

    fclose(movie_file);
}

// list with options
void list(char *lista, char *filtro, int opt)
{
    FILE *movie_file;
    movie m;
    char *temp[MAX*2];

    movie_file = fopen(MOVIE_FILE, "r");
    switch (opt)
    {
    // list by names
    case LISTA_TITULOS:
        while (fread(&m, sizeof(movie), 1, movie_file))
        {
            strcat(lista, "Id -> ");
            strcat(lista, m.id);
            strcat(lista, " | Titulo -> ");
            strcat(lista, m.titulo);
            strcat(lista, "\n");
        }
        break;

    // list by id
    case LISTA_IDS:
        while (fread(&m, sizeof(movie), 1, movie_file))
        {
            if (!strcmp(m.id, filtro))
            {
                strcat(lista, "Titulo -> ");
                strcat(lista, m.titulo);
                strcat(lista, " | Genero -> ");
                strcat(lista, m.genero);
                strcat(lista, " | Diretor -> ");
                strcat(lista, m.diretor);
                strcat(lista, " | Ano Lancamento -> ");
                strcat(lista, m.ano);
                strcat(lista, "\n");
            }
        }

    // list by type
    case LISTA_GENEROS:
        while (fread(&m, sizeof(movie), 1, movie_file))
        {
            strcpy(temp, m.genero);
			char * token = strtok(temp, ",");
            while (token != NULL)
            {
                if (!strcmp(token, filtro))
                {
                    strcat(lista, "Titulo -> ");
                    strcat(lista, m.titulo);
                    strcat(lista, " | Genero -> ");
                    strcat(lista, m.genero);
                    strcat(lista, " | Diretor -> ");
                    strcat(lista, m.diretor);
                    strcat(lista, " | Ano Lancamento -> ");
                    strcat(lista, m.ano);
                    strcat(lista, "\n");
                    break;
                }
                token = strtok(NULL, ",");
            }
        }
        break;
    }
    fclose(movie_file);
}

// filter every command
void controller(char *msg, char *buff)
{
    char aux_msg[MAX];
    strcpy(aux_msg, msg);
    char *split = strtok(msg, SPECIALSEP);

    if (!strcmp(CADASTRO, split))
        cadastra(buff, aux_msg);

    if (!strcmp(GENERO, split)) {
        split = strtok(NULL, " ");
        if (split == NULL) {
            strcpy(buff, "Insira um - separando o comando do ID do filme, e o Genero a ser adicionado\n");
        } else {
            adiciona_genero(buff, split);
        }

    }
    if (!strcmp(LISTA_GENERO, split))
    {
        split = strtok(NULL, "");
        if (split == NULL)
            strcpy(buff, "Insira um - separando o comando do Genero desejado\n");
        else
            list(buff, split, LISTA_GENEROS);
    }
    if (!strcmp(LISTA_ID, split))
    {
        split = strtok(NULL, "");
        if (split == NULL)
            strcpy(buff, "Insira um - separando o comando do ID desejado\n");
        else
            list(buff, split, LISTA_IDS);
    }
    
    if (!strcmp(DELETE, split))
    {
        split = strtok(NULL, "");
        if (split == NULL)
            strcpy(buff, "Insira um - separando o comando do ID a ser deletado\n");
        else
            remove_movie(buff, split);
    }
    if (!strcmp(LISTA_TITULO, split))
        list(buff, split, LISTA_TITULOS);
    if (!strcmp(LISTA_TODOS, split))
        listAll(buff);

}

// intercept message and sends return message
void *mid_controller(int connfd, char *msg, struct sockaddr_in sockfd)
{
    int n;
    char buff[MAX];

    controller(msg, buff);
    n = sizeof(sockfd);
    sendto(connfd, (const char *)buff, strlen(buff), 0, (const struct sockaddr *)&sockfd, n);
    memset(buff, 0, sizeof(buff));

    return;
}

int main()
{
    int length, n;
    struct sockaddr_in client;
    char buffer[MAX];

    struct sockaddr_in server;

    int serverfd;
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);

    // create server
    if (serverfd < 0)
    {
        printf("Falha na criacao do socket\n");
        return 1;
    }
    printf("Socket criado com sucesso!\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(serverfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Falha na conexão com o IP!\n");
        return 1;
    }
    printf("Conexão com o ip realizada.\n");

    while (1)
    {
        length = sizeof(client);
        n = recvfrom(serverfd, (char *)buffer, MAX, 0, (struct sockaddr *)&client, &length);
        buffer[n] = '\0';
        mid_controller(serverfd, buffer, client);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}