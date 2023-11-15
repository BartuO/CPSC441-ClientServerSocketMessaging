#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT_TCP 8080 

int main() {
    int server_fd, socket_tcp; // File descriptors for the server and the new client socket
    struct sockaddr_in tcp_address; // Structure containing an internet address
    int tcp_addrlen = sizeof(tcp_address);
    char buffer_tcp[1024] = {0}; // Buffer to store messages

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    tcp_address.sin_family = AF_INET; // Address family (IPv4)
    tcp_address.sin_addr.s_addr = INADDR_ANY; // Listen on all available interfaces
    tcp_address.sin_port = htons(PORT_TCP); // Port number, converted to network byte order

    // Binding the socket to the address and port number specified above
    if (bind(server_fd, (struct sockaddr *)&tcp_address, sizeof(tcp_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for client connections, with a maximum of 3 pending connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
		std::cout << "Server is listening on port 8080..." << std::endl;


    // Accepting a client connection
    if ((socket_tcp = accept(server_fd, (struct sockaddr *)&tcp_address, (socklen_t *)&tcp_addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Reading the message from the client and printing it to the console
    ssize_t bytesRead = read(socket_tcp, buffer_tcp, 1024);
		if (bytesRead < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    std::cout << "Message received: " << buffer_tcp << std::endl;

    // Sending a response to the client
    send(socket_tcp, buffer_tcp, strlen(buffer_tcp), 0);
    std::cout << "Message sent back" << std::endl;
    
    return 0;
}

