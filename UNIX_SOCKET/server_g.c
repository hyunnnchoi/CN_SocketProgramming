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

    int server_socket, client_socket;
    socklen_t clilen;
    struct sockaddr_un serv_addr, cli_addr;
    char buffer[256];

    // Create a socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        error("Error opening socket");
    }

    // Initialize socket structure
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCKET_PATH);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <
        0) {
        error("Error on binding");
    }

    // Listen for connections
    listen(server_socket, 5);
    clilen = sizeof(cli_addr);

    printf("Server is waiting for connections...\n");

    // Accept a connection
    client_socket =
        accept(server_socket, (struct sockaddr *)&cli_addr, &clilen);
    if (client_socket < 0) {
        error("Error on accept");
    }

    while (1) {
        bzero(buffer, 256);
        int n = read(client_socket, buffer, 255);
        if (n < 0) {
            error("Error reading from socket");
        }
        printf("Client: %s\n", buffer);

        bzero(buffer, 256);
        printf("Server: ");
        fgets(buffer, 255, stdin);

        n = write(client_socket, buffer, strlen(buffer));
        if (n < 0) {
            error("Error writing to socket");
        }
    }

    // Close the sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
