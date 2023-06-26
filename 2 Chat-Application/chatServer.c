#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 5  // Max number of clients is 5
#define BUFFER_SIZE 1024

int main() { 
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        printf("Failed to create socket.");
        exit(1);
    }
    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8888);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("Failed to bind.");
        exit(1);
    }
    // Listen for incoming connections
    listen(serverSocket, MAX_CLIENTS);
    printf("Server listening on port 8888...\n");

    while (1) {
        // Accept connection from client
        clientLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLength);
        if (clientSocket < 0)
        {
            printf("Connection Falure");
            exit(1);
        }
        printf("New client connected: %s\n", inet_ntoa(clientAddress.sin_addr));

        // Handle client messages
        while (1) {
            printf("Print 'bye' to terminate session\n");
            memset(buffer, 0, sizeof(buffer));
            if (read(clientSocket, buffer, sizeof(buffer)) < 0)
            {
                printf("Failed to read from socket");
                exit(1);
            } 
            // Check if client wants to exit
            if (strncmp(buffer, "bye", 4) == 0)
                break;

            // Reply to the client
            printf("Enter your reply: ");
            fgets(buffer, sizeof(buffer), stdin);
            if (write(clientSocket, buffer, strlen(buffer)) < 0)
            {
                printf("Failed to write to socket.");
                exit(1);
            }
        }

        // Close the client socket
        close(clientSocket);
        printf("Client disconnected: %s\n", inet_ntoa(clientAddress.sin_addr));
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
