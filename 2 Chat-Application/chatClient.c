#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        printf("Failed to create socket");
        exit(1);
    }
    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);

    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
    {
        printf("Invalid address");
        exit(1);
    }
    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Failed to connect to server");
        exit(1);
    }    
    printf("Connected to the server.\n");

    while (1) {
        printf("Enter message: ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);

        // Send message to the server
        if (write(clientSocket, buffer, strlen(buffer)) < 0)
        {
            printf("Failed to write to socket");
            exit(1);
        }    
        // Receive reply from the server
        memset(buffer, 0, sizeof(buffer));
        if (read(clientSocket, buffer, sizeof(buffer)) < 0)
        {
            printf("Failed to read from socket");
            exit(1);
        }           
        printf("Server reply: %s\n", buffer);
        // Check if user wants to exit
        if (strncmp(buffer, "exit", 4) == 0)
            break;
    }
    // Close the socket
    close(clientSocket);
    return 0;
}
