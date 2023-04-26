#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "file_system.h"


#define BUFF_SIZE 8192

int create_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

void specify_server_address(struct sockaddr_in *server_addr, char *ip_addr, char *port) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(atoi(port));
    server_addr->sin_addr.s_addr = inet_addr(ip_addr);
}

int connect_to_server(int client_sock, struct sockaddr_in *server_addr) {
    return connect(client_sock, (struct sockaddr*)server_addr, sizeof(struct sockaddr));
}

void send_message(int client_sock, char *buff) {
    int msg_len = strlen(buff);
    if (msg_len == 0) return;
    send(client_sock, buff, msg_len, 0);
}

int receive_message(int client_sock, char *buff) {
    int bytes_received = recv(client_sock, buff, BUFF_SIZE-1, 0);
    buff[bytes_received] = '\0';
    return bytes_received;
}

void process_send_message(int client_sock) {
    char buff[BUFF_SIZE];
    send_message(client_sock, "message");

    while(1) {
        //send message
        printf("\nInsert string to send:");
        memset(buff,'\0',(strlen(buff)+1));
        
        fgets(buff, BUFF_SIZE, stdin);

        send_message(client_sock, buff);

        // if the user enters an empty string
        if(buff[0] == '\n')
            break;
        
        //receive echo reply
        if(receive_message(client_sock, buff) <= 0){
            printf("\nError!Cannot receive data from sever!\n");
            break;
        }
        
        printf("Reply from server: %s", buff);
    }
}

void process_send_file(int client_sock) {
    char buff[BUFF_SIZE] = {0};
    char *file_buffer;

    send_message(client_sock, "file");

    printf("\nInsert file path:");
    memset(buff,'\0',(strlen(buff)+1));
    fgets(buff, BUFF_SIZE, stdin);

    if(file_formatting(buff, &file_buffer) == 0) 
        return;

    send_message(client_sock, file_buffer);
    free(file_buffer);
}

void menu(int client_sock) {
    int choice;
    
    printf("\nMENU");
    printf("\n1. Send message");
    printf("\n2. Send file");
    printf("\nOther to quit!\n");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch(choice) {
        case 1:
            process_send_message(client_sock);
            break;
        
        case 2:
            process_send_file(client_sock);
            break;
        
        default:
            printf("\nGoodbye!");
            close(client_sock);
            exit(0);
            break;
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <IP address> <port number>\n", argv[0]);
        return 1;
    }

    int client_sock;
    struct sockaddr_in server_addr; /* server's address information */
    
    //Step 1: Construct socket
    client_sock = create_socket();
    
    //Step 2: Specify server address
    specify_server_address(&server_addr, argv[1], argv[2]);
    
    //Step 3: Request to connect server
    if(connect_to_server(client_sock, &server_addr) < 0){
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        return 0;
    }
        
    //Step 4: Communicate with server            
    while(1){
        menu(client_sock);
    }
    
    //Step 5: Close socket
    close(client_sock);
    return 0;
}