#include &lt;stdio.h&gt;
#include &lt;string.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;sys/socket.h&gt;
#include &lt;netinet/in.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;unistd.h&gt; // for close()
#define SERVER_IP &quot;127.0.0.1&quot;
#define SERVER_PORT 5400
#define CLIENT_IP &quot;127.0.0.1&quot;
#define CLIENT_PORT 5401
int main() {
struct sockaddr_in client, server;
int sd;
char str[512];
// Clear and set up client address structure
bzero((char *)&amp;client, sizeof(client));
client.sin_family = AF_INET;
client.sin_addr.s_addr = inet_addr(CLIENT_IP);
client.sin_port = htons(CLIENT_PORT);
// Clear and set up server address structure
bzero((char *)&amp;server, sizeof(server));
server.sin_family = AF_INET;
server.sin_addr.s_addr = inet_addr(SERVER_IP);
server.sin_port = htons(SERVER_PORT);
// Create a UDP socket
sd = socket(AF_INET, SOCK_DGRAM, 0);
if (sd &lt; 0) {

perror(&quot;Socket creation failed&quot;);
return 1;
}
// Bind the socket to the client address
if (bind(sd, (struct sockaddr *)&amp;client, sizeof(client)) &lt; 0) {
perror(&quot;Bind failed&quot;);
close(sd);
return 1;
}
printf(&quot;UDP Client is running...\n&quot;);
// Communication loop
do {
printf(&quot;\nEnter a message: &quot;);
scanf(&quot;%s&quot;, str);
// Send the message to the server
sendto(sd, str, strlen(str) + 1, 0, (struct sockaddr *)&amp;server, sizeof(server));
// Exit if the message is &quot;stop&quot;
if (strcmp(str, &quot;stop&quot;) == 0) {
printf(&quot;Stopping the client...\n&quot;);
break;
}
} while (1);
close(sd); // Close the socket
return 0;
}
