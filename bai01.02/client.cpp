#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server_address;
    char file_name[MAX_BUFFER_SIZE];
    char buffer[MAX_BUFFER_SIZE];
    int bytes_read, bytes_sent, bytes_received;
    FILE *file;

    if(argc < 3) {
        printf("Usage: %s <server-ip-address> <file-name>\n", argv[0]);
        exit(1);
    }

    strcpy(file_name, argv[2]);

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Error creating socket\n");
        exit(1);
    }

    //Configure server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    if(inet_pton(AF_INET, argv[1], &server_address.sin_addr) < 0) {
        perror("Error converting server IP address to binary\n");
        exit(1);
    }

    //Connect to server
    if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error connecting to server\n");
        exit(1);
    }

    //Open file
    file = fopen(file_name, "r");
    if(file == NULL) {
        perror("Error opening file\n");
        exit(1);
    }

    //Read file and send data to server
    while((bytes_read = fread(buffer, 1, MAX_BUFFER_SIZE, file)) > 0) {
        bytes_sent = send(sock, buffer, bytes_read, 0);
        if(bytes_sent != bytes_read) {
            perror("Error sending file data\n");
            exit(1);
        }
    }

    //Close file and socket
    fclose(file);
    close(sock);

    return 0;
}
