
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Signal handler to reap zombie processes
void handle_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);
    char buffer[BUFFER_SIZE];

    // Register signal handler for SIGCHLD to avoid zombie processes
    signal(SIGCHLD, handle_sigchld);

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
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

    printf("Concurrent TCP echo server is running on port %d...\n", PORT);

    while (1) {
        // Accept an incoming connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Fork a new process to handle the client
        if (fork() == 0) {
            // Child process
            close(server_fd); // Close the server socket in the child process

            printf("Connected to client\n");

            // Echo loop: receive message and send it back to client
            while (1) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
                
                if (bytes_received <= 0) {
                    printf("Client disconnected\n");
                    break;
                }

                printf("Received from client: %s", buffer);

                // Echo the message back to the client
                send(client_fd, buffer, bytes_received, 0);
            }

            close(client_fd); // Close client socket in the child process
            exit(0); // Exit child process
        }
        
        // Parent process closes client_fd as it's being handled by child
        close(client_fd);
    }

    close(server_fd); // Close server socket
    return 0;
}

