#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#include "data_structured.h"
#include "file_system.h"
#include "string_const.h"

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 20   /* Number of allowed connections */
#define BUFF_SIZE 1024

extern LinkedList* userList;
extern int signInCount;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



/* Receive and echo message to client */
void *echo(void *);

int create_socket();
void bind_address_to_socket(int listen_sock, struct sockaddr_in *server, char *port);
void listen_for_connections(int listen_sock);
int accept_connection(int listen_sock, struct sockaddr_in *client);
int receive_message(int conn_sock, char *recv_data);
void send_message(int conn_sock, char *buff);
void sign_in(int connfed, char* userName, char* password);



int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    NewLinkedList();
    ReadAccountFromFile();

	int listenfd, *connfd;
	struct sockaddr_in server; /* server's address information */
	struct sockaddr_in *client; /* client's address information */
	int sin_size;
	pthread_t tid;
    
    //Step 1: Construct a TCP socket to listen connection request
    listenfd = create_socket();
    
    //Step 2: Bind address to socket
    bind_address_to_socket(listenfd, &server, argv[1]);
    
    //Step 3: Listen request from client
    listen_for_connections(listenfd);
	
	sin_size=sizeof(struct sockaddr_in);
	client = malloc(sin_size);
	while(1){		
		connfd = malloc(sizeof(int));
		if ((*connfd = accept(listenfd, (struct sockaddr *)client, &sin_size)) ==- 1)
			perror("\nError: ");
				
		printf("You got a connection from %s\n", inet_ntoa(client->sin_addr) ); /* prints client's IP */
		
		/* For each client, spawns a thread, and the thread handles the new client */
		pthread_create(&tid, NULL, &echo, connfd);	
	}
	
	close(listenfd);
	return 0;
}

void *echo(void *arg){
	int connfd;
	int bytes_sent, bytes_received;
	char buff[BUFF_SIZE + 1];
    char username[BUFF_SIZE];
    char password[BUFF_SIZE];

	connfd = *((int *) arg);

	free(arg);
	pthread_detach(pthread_self());
	
    while(1){
        bytes_received = receive_message(connfd, buff);

        if (bytes_received < 0)
            perror("\nError: ");
        else if (bytes_received == 0) {
            printf("\nConnection closed.\n");
            break;
        }
        
        sscanf(buff, "%s %s", username, password);

        sign_in(connfd, username, password);
    }
	
	close(connfd);	
}

void sign_in(int connfd, char* userName, char* password)
{
    pthread_mutex_lock(&mutex); // Lock the mutex

    User* foundUser = SearchByUserName(userName);
    if(foundUser == NULL)
        send_message(connfd, LOGIN_STATUS_ACCOUNT_NOT_FOUND);
    else
    {
        if(strcmp(foundUser->password, password)) 
        {
            send_message(connfd, LOGIN_STATUS_WRONG_PASSWORD);
            foundUser->attempts++;
            if(foundUser->attempts >= MAX_ATTEMPTS)
            {
                foundUser->status = BLOCKED;
                UpdateAccountStatusInFile(foundUser);
                send_message(connfd, LOGIN_STATUS_ACCOUNT_BLOCKED);
            }
        }
        else 
        {
            if(foundUser->status == BLOCKED)
                send_message(connfd, LOGIN_STATUS_ACCOUNT_BLOCKED);
            else {
                signInCount++;
                foundUser->isSignIn = true;
                send_message(connfd, LOGIN_STATUS_SUCCESSFUL);
            }
        }
    }

    pthread_mutex_unlock(&mutex); // Unlock the mutex
}


int create_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void bind_address_to_socket(int listen_sock, struct sockaddr_in *server, char *port) {
    bzero(server, sizeof(*server));
    server->sin_family = AF_INET;         
    server->sin_port = htons(atoi(port));
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listen_sock, (struct sockaddr*)server, sizeof(*server));
}

void listen_for_connections(int listen_sock) {
    listen(listen_sock, BACKLOG);
}

int accept_connection(int listen_sock, struct sockaddr_in *client) {
    int sin_size = sizeof(struct sockaddr_in);
    int conn_sock = accept(listen_sock,( struct sockaddr *)client, &sin_size);
    printf("You got a connection from %s\n", inet_ntoa(client->sin_addr));
    return conn_sock;
}

int receive_message(int conn_sock, char *recv_data) {
    int bytes_received = recv(conn_sock, recv_data, BUFF_SIZE-1, 0);
    recv_data[bytes_received] = '\0';
    return bytes_received;
}

void send_message(int connfd, char *buff) {
    int msg_len = strlen(buff);
    if (msg_len == 0) return;
    send(connfd, buff, msg_len, 0);
}
