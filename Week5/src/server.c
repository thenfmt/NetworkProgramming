#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "file_system.h"

#define BACKLOG 2   /* Number of allowed connections */
#define BUFF_SIZE 8192

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

void send_message(int conn_sock, char *recv_data, int bytes_received) {
    send(conn_sock, recv_data, bytes_received, 0);
}


void string_validation(char *str) {
    char digits[strlen(str)], letters[strlen(str)];

    int i, j = 0, k = 0;

    for (i = 0; i < strlen(str); i++) {
        if(str[i] == '\n') {
            if(i != 0)
                continue;
            else 
                return;
        }

        if (isdigit(str[i])) {
            digits[j] = str[i];
            j++;
        } else if (isalpha(str[i])) {
            letters[k] = str[i];
            k++;
        } else {
            strcpy(str, "error\n");
            return;
        }
    }

    digits[j] = '\0';
    letters[k] = '\0';

    sprintf(str, "%s\n%s\n", digits, letters);
}

void process_recv_message(int conn_sock) {
    char recv_data[BUFF_SIZE];
    int bytes_received;
    
    printf("\n----- send message ------");

    while(1) {
        //receives message from client
        bytes_received = receive_message(conn_sock, recv_data);
        if (bytes_received <= 0){
            printf("\nConnection closed");
            break;
        }
        else{
            printf("\nReceive: %s ", recv_data);
        }
        
        // if the user enters an empty string
        if(recv_data[0] == '\n')
            break;

        //echo to client
        string_validation(recv_data);
        send_message(conn_sock, recv_data, bytes_received);
    }
    printf("\n------------------------\n");
}

void process_recv_file(int conn_sock) {
    char recv_data[BUFF_SIZE];
    int bytes_received;
    
    printf("\n------ send file ------");

    while(1) {
        bytes_received = receive_message(conn_sock, recv_data);
        if (bytes_received <= 0){
            printf("\nConnection closed");
            break;
        }
        else{
            printf("\nReceive: %s ", recv_data);

            if(create_file_with_format(recv_data) == 0)
                return;

            send_message(conn_sock, recv_data, bytes_received);
            break;
        }
    }
    printf("\n------------------------\n");
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    char recv_data[BUFF_SIZE];
    int listen_sock, conn_sock; /* file descriptors */
    int bytes_sent, bytes_received;
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    
    //Step 1: Construct a TCP socket to listen connection request
    listen_sock = create_socket();
    
    //Step 2: Bind address to socket
    bind_address_to_socket(listen_sock, &server, argv[1]);
    
    //Step 3: Listen request from client
    listen_for_connections(listen_sock);
    
    //Step 4: Communicate with client
    while(1){
        //accept request
        conn_sock = accept_connection(listen_sock, &client);
        
        //start conversation
        while(1){
            //receives message from client
            bytes_received = receive_message(conn_sock, recv_data);
            if (bytes_received <= 0){
                printf("\nConnection closed");
                break;
            }
            
            if(strcmp(recv_data, "message") == 0) 
                process_recv_message(conn_sock);
            else if(strcmp(recv_data, "file") == 0)
                process_recv_file(conn_sock);
            else 
                break;
            
        }//end conversation
        close(conn_sock);    
    }
    
    close(listen_sock);
    return 0;
}