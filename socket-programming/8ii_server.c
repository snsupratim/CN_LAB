#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Clear and configure server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP Echo server is running on port %d...\n", PORT);

    // Main loop to handle incoming messages
    while (1) {
        // Receive message from a client
        int len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (len < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[len] = '\0';  // Null-terminate the received string
        printf("Received from client: %s\n", buffer);

        // Send the received message back to the client
        sendto(sockfd, buffer, len, 0, (struct sockaddr *)&client_addr, client_len);
        printf("Echoed back to client: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}

