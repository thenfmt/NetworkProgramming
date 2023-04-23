#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024

int main(int argc, char *argv[]) {
    int client_sock;
    char buff[BUFF_SIZE];
    struct sockaddr_in server_addr;
    int bytes_sent, bytes_received, sin_size;

    if (argc != 3) {
        printf("Usage: %s <IP address> <port number>\n", argv[0]);
        return 1;
    }

    // Step 1: Construct a UDP socket
    if ((client_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("\nError: ");
        return 1;
    }

    // Step 2: Define the address of the server
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // send a message to server
    sin_size = sizeof(struct sockaddr);
    strcpy(buff, "sender");
    bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
    if (bytes_sent < 0) {
        perror("Error: ");
        close(client_sock);
        return 1;
    }

    // Step 3: Communicate with server
    while (1) {
        printf("\nInsert string to send:");
        memset(buff, 0, BUFF_SIZE);
        fgets(buff, BUFF_SIZE, stdin);
        

        sin_size = sizeof(struct sockaddr);

        bytes_sent = sendto(client_sock, buff, strlen(buff), 0, (struct sockaddr *) &server_addr, sin_size);
        if (bytes_sent < 0) {
            perror("Error: ");
            close(client_sock);
            return 1;
        }


        if(buff[0] == '\n') {
            printf("Stop\n");
            exit(0);
        }
    }

    close(client_sock);
    return 0;
}
