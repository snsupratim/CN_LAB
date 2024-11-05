#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set up server address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    printf("Echo server is listening on port %d...\n", PORT);
    
    while (1) {
        // Accept a connection from a client
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        
        printf("Connected to client\n");
        
        // Echo loop: receive message and send it back to client
        while (1) {
            memset(buffer, 0, BUFFER_SIZE);
            int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
            
            if (bytes_received == 0) {
                printf("Client disconnected\n");
                break;
            }
            if (bytes_received < 0) {
                perror("Receive failed");
                break;
            }
            
            printf("Received: %s", buffer);
            
            // Echo the message back to the client
            send(client_fd, buffer, bytes_received, 0);
        }
        
        // Close the client socket
        close(client_fd);
    }
    
    // Close the server socket
    close(server_fd);
    return 0;
}
