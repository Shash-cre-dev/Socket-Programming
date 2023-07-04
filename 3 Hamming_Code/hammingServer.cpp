#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<math.h>
using namespace std;
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

// Function to calculate parity
char calcHam(int position, int totalBits, char code[]) {
    int count = 0, i, j;
    i = totalBits - position;
    while (i >= 0) {
        for (j = i; j > i- position; j--) {
            if (code[j] == '1') {
                count++;
            }
        }
        i=i-2*position;
    }
    // Check if count is even
    if (count%2 == 0) {
        return '0';
    } else {
        return '1';
    }
    
}
int checkHamming(char dataatrec[]) {
	int size = 0;
	int i = 0;
	while (dataatrec[i] != NULL) {
		size++;
		i++;
	}
	int k = 0;
	int flag = 1;
    int errorBit;
    for (i = 0; i < size; i++) {
        if (i == ((int)pow(2,k)-1)) {
        	if (calcHam(i+1, size, dataatrec) != '0') {
        		flag = 0;
                break;
        	}
            k++;
        }    
    }
    if (flag == 1) {
        cout << "No error" << endl;
    } else {
        cout << "Error in the hamming code" << endl;
    }
    return flag;
}
void error(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
        error("Failed to create socket.");

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8882);

    // Bind the socket
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        error("Failed to bind.");

    // Listen for incoming connections
    listen(serverSocket, MAX_CLIENTS);

    printf("Server listening on port 8888...\n");

    while (1) {
        // Accept connection from client
        clientLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLength);
        if (clientSocket < 0)
            error("Failed to accept connection.");

        printf("New client connected: %s\n", inet_ntoa(clientAddress.sin_addr));

        memset(buffer, 0, sizeof(buffer));
        if (read(clientSocket, buffer, sizeof(buffer)) < 0)
            error("Failed to read from socket.");
        checkHamming(buffer);
        
        // Close the client socket
        close(clientSocket);
        printf("Client disconnected: %s\n", inet_ntoa(clientAddress.sin_addr));
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
