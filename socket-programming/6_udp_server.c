#include &lt;stdio.h&gt;
#include &lt;string.h&gt;
#include &lt;sys/socket.h&gt;
#include &lt;sys/types.h&gt;
#include &lt;netinet/in.h&gt;
#include &lt;arpa/inet.h&gt;
#include &lt;unistd.h&gt; // for close()
#define SERVER_IP &quot;127.0.0.1&quot;
#define SERVER_PORT 5400
int main() {
struct sockaddr_in client, server;
int sd;
socklen_t clen = sizeof(client);
char str[512];
// Clear the server structure
bzero((char *)&amp;server, sizeof(server));
// Set server information
server.sin_family = AF_INET;
server.sin_addr.s_addr = inet_addr(SERVER_IP);
server.sin_port = htons(SERVER_PORT);
// Create a UDP socket
sd = socket(AF_INET, SOCK_DGRAM, 0);
if (sd &lt; 0) {
perror(&quot;Socket creation failed&quot;);
return 1;
}
// Bind the socket to the server address
if (bind(sd, (struct sockaddr *)&amp;server, sizeof(server)) &lt; 0) {
perror(&quot;Bind failed&quot;);
close(sd);
return 1;
}
printf(&quot;UDP Server is running...\n&quot;);
while (1) {
// Clear the buffer and receive message from client
memset(str, 0, 512);
recvfrom(sd, str, 512, 0, (struct sockaddr *)&amp;client, &amp;clen);
printf(&quot;\nReceived message: %s&quot;, str);

printf(&quot;\nClient&#39;s IP: %s&quot;, inet_ntoa(client.sin_addr));
printf(&quot;\nClient&#39;s port: %u&quot;, ntohs(client.sin_port));
// Exit if the message is &quot;stop&quot;
if (strcmp(str, &quot;stop&quot;) == 0) {
printf(&quot;\nStopping the server...\n&quot;);
break;
}
}
close(sd); // Close the socket
return 0;
}
