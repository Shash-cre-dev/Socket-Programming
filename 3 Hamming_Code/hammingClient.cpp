#include <cstdio>
#include<iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
using namespace std;
#define BUFFER_SIZE 1024
char input[32];
char code[32];
char calc_ham(int position, int totalBits) {
    int count = 0, i, j;
    i = totalBits - position;
    while (i >= 0) {
        // Counter number of 1's
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
void hamming() {
    int n, i, parityBits=0, totalBits = 0;
    cout << "Enter number of bits in data (maximum 8): ";
    cin >> n;
    for (i = 0; i < n; i++) {
        cin >> input[i];
    }
    i = 0;
    // Hamming code redundant bits criteria: 2^r >= m+r+1
    while (n > (int)pow(2,i)-(i+1)) {
        i++;
        parityBits++;
    }
    totalBits = parityBits + n;
    int j = 0, k = 0, p = n-1;
    for (i = 0; i < totalBits; i++) {
        if (i == ((int)pow(2,k)-1)) {
            code[totalBits - i - 1] = '0'; // Set all parity bits to 0;
            k++;
        }
        else {
            code[totalBits - i - 1] = input[p];
            j++;
            p--;
        }
    }
    // Calculating the hamming code
    for (i = 0; i < parityBits; i++) {
        int position = (int)pow(2,i);
        char value = calc_ham(position, totalBits);
        code[totalBits - position] = value;
    }
}
void error(const char *message) {
    perror(message);
    exit(1);
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddress;
    char *buffer;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
        error("Failed to create socket.");

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8882);

    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0)
        error("Invalid address.");

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
        error("Failed to connect to server.");

    printf("Connected to the server.\n");
    hamming();
    // Send message to the server
    if (write(clientSocket, code, strlen(code)) < 0)
        error("Failed to write to socket.");
	else
	    cout << "Sent encoded hamming code " << code << " to server successfully "<< endl;
    close(clientSocket);

    return 0;
}
