#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define PORT_TCP 8080

int main() {
    int tcp_socket = 0; // File descriptor for the socket
    char buffer_tcp[1024] = {0}; // Buffer to store messages

    // Creating socket file descriptor
    if ((tcp_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }

    struct sockaddr_in tcp_address ;
    tcp_address.sin_family = AF_INET; // Address family (IPv4)
    tcp_address.sin_port = htons(PORT_TCP); // Port number to connect to, converted to network byte order
		inet_pton(AF_INET, "127.0.0.1", &tcp_address.sin_addr); // Converting IPv4 and IPv6 addresses from text to binary form
  

    // Connecting to the server
    if (connect(tcp_socket, (struct sockaddr *)&tcp_address, sizeof(tcp_address)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }

    // Sending a message to the server
    send(tcp_socket, "Hello from client", strlen("Hello from client"), 0);
    std::cout << "Hello message sent" << std::endl;

    // Reading the response from the server and printing it to the console
    read(tcp_socket, buffer_tcp, 1024);
    std::cout << "Message received: " << buffer_tcp << std::endl;
    
    return 0;
}