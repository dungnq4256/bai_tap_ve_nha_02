#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main() {
    int server_sock, client_sock, bytes_received;
    struct sockaddr_in server_address, client_address;
    char buffer[MAX_BUFFER_SIZE];
    int count = 0;

    //Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock < 0) {
        perror("Error creating socket\n");
        exit(1);
    }

    //Configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);

    //Bind socket to address
    if(bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket to address\n");
        exit(1);
    }

    //Listen
    if(listen(server_sock, 5) < 0) {
        perror("Error listening on socket\n");
        exit(1);
    }

    printf("Server started, waiting for connections...\n");

    while(1) {
        //Accept connection
        int client_address_length = sizeof(client_address);
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, (socklen_t *)&client_address_length);
        if(client_sock < 0) {
            perror("Error accepting connection\n");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        //Receive data from client
        while((bytes_received = recv(client_sock, buffer, MAX_BUFFER_SIZE, 0)) > 0) {
            char *p = buffer;
            while((p = strstr(p, "0123456789")) != NULL) {
                count++;
                p += strlen("0123456789");
            }
        }

        //Print result
        printf("Number of occurrences of '0123456789': %d\n", count);

        //Close client socket
        close(client_sock);
    }

    //Close server socket
    close(server_sock);

    return 0;
}
