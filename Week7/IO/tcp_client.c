#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "string_const.h"

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5550
#define BUFF_SIZE 1024


int create_socket();
void specify_server_address(struct sockaddr_in *server_addr, char *ip_addr, char *port);
int connect_to_server(int client_sock, struct sockaddr_in *server_addr);
void send_message(int client_sock, char *buff);
int receive_message(int client_sock, char *buff);


int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <IP address> <port number>\n", argv[0]);
        return 1;
    }

    bool is_logged_in = false;
    int client_sock;
    struct sockaddr_in server_addr; /* server's address information */
    char buff[BUFF_SIZE * 2];
    char username[BUFF_SIZE];
    char password[BUFF_SIZE];

    // Step 1: Construct socket
    client_sock = create_socket();

    // Step 2: Specify server address
    specify_server_address(&server_addr, argv[1], argv[2]);

    // Step 3: Request to connect server
    if (connect_to_server(client_sock, &server_addr) < 0) {
        printf("\nError! Can not connect to server! Client exit immediately!\n");
        return 0;
    }

    // Step 4: Communicate with server
    while (1) {
        if (is_logged_in)
            printf("\nYou're logged in. Enter new info to switch to another account\n");

        is_logged_in = false;

        printf("\nEnter user name: ");
        scanf("%s", username);

        printf("\nEnter user password: ");
        scanf("%s", password);

        memset(buff, '\0', (strlen(buff)));
        sprintf(buff, "%s %s", username, password);

        send_message(client_sock, buff);

        // Receive echo reply
        if (receive_message(client_sock, buff) <= 0) {
            printf("\nError! Cannot receive data from server!\n");
            break;
        }

        printf("\n--------------------------------");
        printf("\nReply from server: %s\n", buff);
        printf("\n--------------------------------\n");

        if (strcmp(buff, LOGIN_STATUS_SUCCESSFUL) == 0)
            is_logged_in = true;
    }

    // Step 5: Close socket
    close(client_sock);
    return 0;
}


int create_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void specify_server_address(struct sockaddr_in *server_addr, char *ip_addr, char *port) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(atoi(port));
    server_addr->sin_addr.s_addr = inet_addr(ip_addr);
}

int connect_to_server(int client_sock, struct sockaddr_in *server_addr) {
    return connect(client_sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr));
}

void send_message(int client_sock, char *buff) {
    int msg_len = strlen(buff);
    if (msg_len == 0) return;
    send(client_sock, buff, msg_len, 0);
}

int receive_message(int client_sock, char *buff) {
    int bytes_received = recv(client_sock, buff, BUFF_SIZE - 1, 0);
    buff[bytes_received] = '\0';
    return bytes_received;
}
