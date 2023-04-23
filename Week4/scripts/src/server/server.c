/*UDP Echo Server*/
#include <stdio.h>          /* These are the usual header files */
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "data_structured.h"
#include "file_system.h"
#include "functionality.h"


#define BUFF_SIZE 1024
#define MAX_CLIENTS 2

extern LinkedList* userList;
extern int signInCount;


// declare functions
void drop_new_line(char *str);
int string_validation(char *str);
void process_user_input(int server_sock, struct sockaddr_in* client, int sin_size, User **currUser, char* buff);
void construct_socket(int* server_sock, int argc, char* argv[]);
int accept_client(int server_sock, struct sockaddr_in* client, int* sin_size);
int receive_message(int server_sock, struct sockaddr_in* client, int sin_size, char* buff);
int send_message(int server_sock, struct sockaddr_in* client, int sin_size, char* buff);
void server(int argc, char* argv[]);
void login(int server_sock, struct sockaddr_in* client, int sin_size, User **currUser, char* buff);


int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    signInCount = 0;
    NewLinkedList();
    ReadAccountFromFile();

    server(argc, argv);

    return 0;
}

void drop_new_line(char *str) {
    int len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int string_validation(char *str) {
    char digits[strlen(str)], letters[strlen(str)];

    int i, j = 0, k = 0;

    for (i = 0; i < strlen(str); i++) {
        if(str[i] == '\n') {
            if(i != 0)
                continue;
            else {
                printf("Stop\n");
                return 0;
            }

        }

        if (isdigit(str[i])) {
            digits[j] = str[i];
            j++;
        } else if (isalpha(str[i])) {
            letters[k] = str[i];
            k++;
        } else {
            strcpy(str, "error\n");
            return 1;
        }
    }

    digits[j] = '\0';
    letters[k] = '\0';

    sprintf(str, "%s\n%s\n", digits, letters);

    return 1;
}

void login(int server_sock, struct sockaddr_in* client, int sin_size, User **currUser, char* buff) {
    if(*currUser == NULL) {
        User* found_user = SearchByUserName(buff);
    
        if(found_user != NULL) {
            send_message(server_sock, client, sin_size, "insert password\n");
            (*currUser) = found_user;
        } else {
            send_message(server_sock, client, sin_size, "User not found\n");
        }
    } else {
        if(strcmp(buff, (*currUser)->password) == 0) {
            if((*currUser)->status == ACTIVE) {
                send_message(server_sock, client, sin_size, "OK\n");
                signInCount++;
                (*currUser)->isSignIn = true;
            } else {
                send_message(server_sock, client, sin_size, "Account not ready\n");
                (*currUser) = NULL;
            }
        } else {
            send_message(server_sock, client, sin_size, "NOT OK\n");
            (*currUser)->attempts++;
            if((*currUser)->attempts >= MAX_ATTEMPTS) {
                (*currUser)->status = BLOCKED;
                UpdateAccountStatusInFile((*currUser));
                send_message(server_sock, client, sin_size, "Account is blocked\n");
            }
            (*currUser) = NULL;
        }
    }
}

void change_password(int server_sock, struct sockaddr_in* client, int sin_size, User **currUser, char* buff) {
    if(strcmp(buff, "bye") == 0) {
        char message[100];
        sprintf(message, "Goodbye %s\n", (*currUser)->userName);
        send_message(server_sock, client, sin_size, message);

        (*currUser)->isSignIn = false;
        signInCount--;
        (*currUser) = NULL;
    } else {
        strcpy((*currUser)->password, buff);
        UpdateAccountPassword((*currUser));
        string_validation(buff);
        send_message(server_sock, client, sin_size, buff);
    }
}

void process_user_input(int server_sock, struct sockaddr_in* client, int sin_size, User **currUser, char* buff) {
    if(buff[0] == '\n') {
        send_message(server_sock, client, sin_size, buff);
        exit(0);
    }
    
    drop_new_line(buff);

    if(signInCount == 0) {
        login(server_sock, client, sin_size, currUser, buff);
    } else {
        change_password(server_sock, client, sin_size, currUser, buff);
    }
}

void construct_socket(int* server_sock, int argc, char* argv[]) {
    if ((*server_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("\nError: ");
        exit(0);
    }
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server.sin_zero),8);
    if (bind(*server_sock, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1) {
        perror("\nError: ");
        exit(0);
    }
}

int accept_client(int server_sock, struct sockaddr_in* client, int* sin_size) {
    int bytes_received = 0;
    *sin_size = sizeof(struct sockaddr_in);
    char buff[BUFF_SIZE];
    memset(buff, 0, BUFF_SIZE);
    bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr*) client, sin_size);
    if (strcmp(buff, "sender") == 0) {
        printf("sender: %s - %d\n", inet_ntoa(client->sin_addr), ntohs(client->sin_port));
        return 1;
    } else if (strcmp(buff, "receiver") == 0) {
        printf("receiver: %s - %d\n", inet_ntoa(client->sin_addr), ntohs(client->sin_port));
        return 2;
    } else {
        printf("undefined client!\n");
        return 0;
    }
}

int receive_message(int server_sock, struct sockaddr_in* client, int sin_size, char* buff) {
    int bytes_received = 0;
    memset(buff, 0, BUFF_SIZE);
    bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr*) client, &sin_size);
    return bytes_received;
}

int send_message(int server_sock, struct sockaddr_in* client, int sin_size, char* buff) {
    int bytes_sent = 0;
    bytes_sent = sendto(server_sock, buff, strlen(buff), 0, (struct sockaddr*) client, sin_size);
    if (bytes_sent < 0)
        perror("\nError: ");
    return bytes_sent;
}

void server(int argc, char* argv[]) {
    int server_sock; /* file descriptors */
    char buff[BUFF_SIZE];
    struct sockaddr_in client[MAX_CLIENTS]; /* clients' address information */
    int sin_size[MAX_CLIENTS];
    int num_clients = 0;

    // Step 1: Construct a UDP socket
    construct_socket(&server_sock, argc, argv);

    // Step 2: Accept and process clients
    int sender_index = -1;
    int receiver_index = -1;

    User *currUser = NULL;

    while (1) {
        if (num_clients < MAX_CLIENTS) {
            int client_type = accept_client(server_sock, &client[num_clients], &sin_size[num_clients]);
            if (client_type == 1) {
                sender_index = sender_index == -1 ? num_clients++ : num_clients;
            } else if (client_type == 2) {
                receiver_index = receiver_index == -1 ? num_clients++ : num_clients;
            }
        } else {
            int bytes_received = receive_message(server_sock, &client[sender_index], sin_size[sender_index], buff);
            process_user_input(server_sock, &client[receiver_index], sin_size[receiver_index], &currUser, buff);
        }
    }
}