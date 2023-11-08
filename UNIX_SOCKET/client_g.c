#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/chat_socket"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IP>\n", argv[0]);
        exit(1);
    }

    int client_socket;
    struct sockaddr_un serv_addr;
    char buffer[256];

    // Create a socket
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        error("Error opening socket");
    }

    // Initialize socket structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0) {
        error("Error connecting to server");
    }

    while (1) {
        bzero(buffer, 256);
        printf("Client: ");
        fgets(buffer, 255, stdin);

        int n = write(client_socket, buffer, strlen(buffer));
        if (n < 0) {
            error("Error writing to socket");
        }

        bzero(buffer, 256);
        n = read(client_socket, buffer, 255);
        if (n < 0) {
            error("Error reading from socket");
        }
        printf("Server: %s\n", buffer);
    }

    // Close the socket
    close(client_socket);

    return 0;
}
