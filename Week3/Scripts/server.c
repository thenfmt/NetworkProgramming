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

#define BUFF_SIZE 1024
#define MAX_CLIENTS 2

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

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <port number>\n", argv[0]);
        exit(1);
    }

    int server_sock; /* file descriptors */
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client[MAX_CLIENTS]; /* clients' address information */
    int sin_size[MAX_CLIENTS];
    int num_clients = 0;

    //Step 1: Construct a UDP socket
    if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
        perror("\nError: ");
        exit(0);
    }

    //Step 2: Bind address to socket
    server.sin_family = AF_INET;         
    server.sin_port = htons(atoi(argv[1]));   /* Convert port number from command line argument to network byte order */
    server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */   
    bzero(&(server.sin_zero),8); /* zero the rest of the structure */

    if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
        perror("\nError: ");
        exit(0);
    }   

    //Step 3: Communicate with clients
    int sender_index = -1;
    int receiver_index = -1;

    while(1) {
        memset(buff, 0, BUFF_SIZE);
        if(num_clients < MAX_CLIENTS) {
            sin_size[num_clients] = sizeof(struct sockaddr_in);
            bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client[num_clients], &sin_size[num_clients]);
            if(strcmp(buff, "sender") == 0) {
                printf("sender: %s - %d\n", inet_ntoa(client[num_clients].sin_addr), ntohs(client[num_clients].sin_port));
                sender_index = sender_index == -1 ? num_clients++ : num_clients;
            } else if(strcmp(buff, "receiver") == 0) {
                printf("receiver: %s - %d\n", inet_ntoa(client[num_clients].sin_addr), ntohs(client[num_clients].sin_port));
                receiver_index = receiver_index == -1 ? num_clients++ : num_clients;
            } else
                printf("undefine client!\n");
            
        } else {
            bytes_received = recvfrom(server_sock, buff, BUFF_SIZE-1, 0, (struct sockaddr *) &client[sender_index], &sin_size[sender_index]);

            if(string_validation(buff) == 0) {
                strcpy(buff, "exit");
                bytes_sent = sendto(server_sock, buff, strlen(buff), 0, (struct sockaddr *) &client[receiver_index], sin_size[receiver_index]);
                if (bytes_sent < 0)
                        perror("\nError: ");
                exit(0);
            }

            bytes_sent = sendto(server_sock, buff, strlen(buff), 0, (struct sockaddr *) &client[receiver_index], sin_size[receiver_index]);
            if (bytes_sent < 0)
                    perror("\nError: ");
        }
    }

    close(server_sock);
    return 0;
}
