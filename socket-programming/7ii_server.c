
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo server is running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Receive message from client
        int bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            continue;
        }

        printf("Received message: %s", buffer);

        // Echo the message back to the client
        sendto(server_fd, buffer, bytes_received, 0, (struct sockaddr *)&client_address, client_len);
    }

    // Close the socket (though we usually never reach here in an iterative server)
    close(server_fd);
    return 0;
}
