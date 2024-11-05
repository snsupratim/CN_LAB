

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define WINDOW_SIZE 4
#define BUFFER_SIZE 1024
#define PORT 8080
#define SERVER_IP "127.0.0.1"
// Structure to hold packet information
struct packet {
int seq_num;
char data[BUFFER_SIZE];
};
int main() {
int sockfd;
struct sockaddr_in server_addr;
socklen_t addr_len = sizeof(struct sockaddr_in);
char buffer[BUFFER_SIZE];
int seq_num = 0;
// Create UDP socket
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
memset(&server_addr, 0, sizeof(server_addr));
// Server information

server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
server_addr.sin_port = htons(PORT);
while (1) {
// Create packet
struct packet send_packet;
sprintf(send_packet.data, "Message %d", seq_num);
send_packet.seq_num = seq_num;
// Send packet
sendto(sockfd, &send_packet, sizeof(send_packet), 0, (const struct
sockaddr *)&server_addr, addr_len);
printf("Sent: %s\n", send_packet.data);
// Receive acknowledgment
struct packet ack_packet;
int len = recvfrom(sockfd, &ack_packet, sizeof(ack_packet), 0, NULL,
NULL);
if (len <  0) {
perror("Receive failed");
exit(EXIT_FAILURE);
}
// Check if acknowledgment matches sent sequence number
if (ack_packet.seq_num == seq_num) {
printf("Received acknowledgment for sequence number %d\n",
ack_packet.seq_num);
seq_num++;
} else {
printf("Received incorrect acknowledgment: %d\n", ack_packet.seq_num);
}
// Simulate delay
sleep(1);
}
close(sockfd);
return 0;
}
