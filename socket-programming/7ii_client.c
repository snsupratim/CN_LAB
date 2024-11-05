#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    socklen_t server_len = sizeof(server_address);

    // Create socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type messages to send:\n");

    while (1) {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        sendto(sockfd, buffer, strlen(buffer), 0, (const struct sockaddr *)&server_address, server_len);

        // Receive echoed message from server
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &server_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            break;
        }

        buffer[bytes_received] = '\0';  // Null-terminate the received message
        printf("Server: %s", buffer);
    }

    // Close the socket
    close(sockfd);
    return 0;
}

