#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#include "data_structured.h"
#include "file_system.h"
#include "string_const.h"

#define PORT 5550   /* Port that will be opened */ 
#define BACKLOG 20   /* Number of allowed connections */
#define BUFF_SIZE 1024
#define MAX_CLIENTS 30

extern LinkedList* userList;
extern int signInCount;


/* Receive and process message from client */
void process_message(int client_sock, char* recv_data);
void sign_in(int client_sock, char* userName, char* password);
void send_message(int client_sock, char* message);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    NewLinkedList();
    ReadAccountFromFile();

    int listen_sock, client_sock;
    struct sockaddr_in server;
    fd_set master_fds, read_fds;
    int fdmax;
    int i;

    // Step 1: Construct a TCP socket to listen to connection requests
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Step 2: Bind address to socket
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));

    if (bind(listen_sock, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Step 3: Listen for client connections
    if (listen(listen_sock, BACKLOG) == -1) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Initialize the set of active sockets
    FD_ZERO(&master_fds);
    FD_ZERO(&read_fds);
    FD_SET(listen_sock, &master_fds);
    fdmax = listen_sock;

    // Main event loop
    while (1) {
        read_fds = master_fds; // Copy the set of active sockets
        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        // Iterate through the existing connections looking for data to read
        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == listen_sock) {
                    // New connection request
                    struct sockaddr_in client;
                    socklen_t client_len = sizeof(client);
                    client_sock = accept(listen_sock, (struct sockaddr*)&client, &client_len);
                    if (client_sock == -1) {
                        perror("accept failed");
                        continue;
                    }
                    printf("New connection, socket fd is %d, IP is: %s, port: %d\n",
                           client_sock, inet_ntoa(client.sin_addr), ntohs(client.sin_port));

                    FD_SET(client_sock, &master_fds); // Add the new client socket to the set
                    if (client_sock > fdmax) {
                        fdmax = client_sock; // Update the maximum file descriptor
                    }
                } else {
                    // Data from an existing client
                    char recv_data[BUFF_SIZE];
                    int bytes_received = recv(i, recv_data, sizeof(recv_data), 0);
                    if (bytes_received <= 0) {
                        // Connection closed or error occurred
                        if (bytes_received == 0) {
                            // Connection closed by client
                            printf("Socket %d hung up\n", i);
                        } else {
                            // Error occurred
                            perror("recv failed");
                        }

                        // Close the socket and remove it from the set
                        close(i);
                        FD_CLR(i, &master_fds);
                    } else {
                        // Process the received message
                        recv_data[bytes_received] = '\0';
                        process_message(i, recv_data);
                    }
                }
            }
        }
    }

    // Cleanup and close all sockets
    for (i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &master_fds)) {
            close(i);
        }
    }

    return 0;
}

void process_message(int client_sock, char* recv_data) {
    char username[BUFF_SIZE];
    char password[BUFF_SIZE];

    sscanf(recv_data, "%s %s", username, password);
    sign_in(client_sock, username, password);
}

void send_message(int client_sock, char* message)
{
    if (send(client_sock, message, strlen(message), 0) == -1) {
        perror("send failed");
    }
}


void sign_in(int client_sock, char* userName, char* password)
{
    User* foundUser = SearchByUserName(userName);
    if(foundUser == NULL)
        send_message(client_sock, LOGIN_STATUS_ACCOUNT_NOT_FOUND);
    else
    {
        if(strcmp(foundUser->password, password)) 
        {
            send_message(client_sock, LOGIN_STATUS_WRONG_PASSWORD);
            foundUser->attempts++;
            if(foundUser->attempts >= MAX_ATTEMPTS)
            {
                foundUser->status = BLOCKED;
                UpdateAccountStatusInFile(foundUser);
                send_message(client_sock, LOGIN_STATUS_ACCOUNT_BLOCKED);
            }
        }
        else 
        {
            if(foundUser->status == BLOCKED)
                send_message(client_sock, LOGIN_STATUS_ACCOUNT_BLOCKED);
            else if(foundUser->isSignIn)
            {
                send_message(client_sock, LOGIN_STATUS_ACCOUNT_IS_ALREADY_LOGGED_IN);
            }
            else 
            {
                signInCount++;
                foundUser->isSignIn = true;
                send_message(client_sock, LOGIN_STATUS_SUCCESSFUL);
            }
        }
    }
}