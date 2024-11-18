#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
int main() {
int pipe1[2];
int pipe2[2];
char buffer[100];
char combined_message[100];
int i = 0, j = 0;
if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
fprintf(stderr, "Error creating pipes: %s\n", strerror(errno));
return 1;
}
int id = fork();
if (id == -1) {
fprintf(stderr, "Error forking: %s\n", strerror(errno));
return 1;
}
if (id == 0) {
close(pipe1[1]);
close(pipe2[0]);
read(pipe1[0], buffer, sizeof(buffer));
printf("Child received: %s\n", buffer);
char response[50];
printf("enter string 2(child says!):\n");
scanf("%s",response);
write(pipe2[1], response, strlen(response) + 1);
close(pipe1[0]);
close(pipe2[1]);
} else {
close(pipe1[0]);
close(pipe2[1]);
char message[50];
printf("Enter string 1:\n");
scanf("%s",message);
write(pipe1[1], message, strlen(message) + 1);
printf("Parent says: %s\n", message);
read(pipe2[0], buffer, sizeof(buffer));
printf("Parent received: %s\n", buffer);
for (i = 0; message[i] != '\0'; i++) {
combined_message[i] = message[i];
}
combined_message[i] = ' ';
i++;
for (j = 0; buffer[j] != '\0'; j++, i++) {
combined_message[i] = buffer[j];
}
combined_message[i] = '\0';
close(pipe1[1]);
close(pipe2[0]);
wait(NULL);
printf("%s\n", combined_message);
}
return 0;
}
