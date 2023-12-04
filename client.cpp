#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <limits>

#define PORT_TCP 8080
#define PORT_UDP_S 8091
#define PORT_UDP_R 8092
#define BUFFER_SIZE_TCP 6144

using namespace std;


//Name: Bartu Okan
//UCID: 30150180
//Tutorial: T04

//To Compile: g++ -o client client.cpp
//To Run: ./client


//Sending message over TCP
void sendMessage(int tcp_socket, const char* message){
    // Sending a message to the server
    send(tcp_socket, message, strlen(message), 0);
    std::cout << "Message sent over TCP: " << message <<  std::endl;
}

//Receiving message over TCP
void receieveMessage(int tcp_socket, char* buffer_tcp, int bufferSize){
    int bytesRead = read(tcp_socket, buffer_tcp, bufferSize - 1);
    if (bytesRead < 0) {
        std::cerr << "Error reading from server" << std::endl;
    } else {
        buffer_tcp[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Message received over TCP: " << buffer_tcp << std::endl;
    }
}

//Sending message over UDP
int sendMessageUDP(int udp_socket, const char* message, struct sockaddr_in& address){
    int bytesSent = sendto(udp_socket, message, strlen(message), 0, (struct sockaddr*)&address, sizeof(address));
    if (bytesSent == -1) {
        std::cerr << "Failed to send data to the server" << std::endl;
        return -1;
    }
    std::cout << "Message sent over UDP: " << message << std::endl;
    return 1;

    
}

//Receiving message over UDP
int receieveMessageUDP(int udp_socket, char* buffer_udp, int bufferSize, struct sockaddr_in& address, socklen_t& addrlen){
    int bytesReceived = recvfrom(udp_socket, buffer_udp, bufferSize, 0, (struct sockaddr*)&address, &addrlen);
    if (bytesReceived < 0) {
        std::cerr << "Failed to receive data" << std::endl;
        return -1;
    }
    buffer_udp[bytesReceived] = '\0'; // Null-terminate the received data
    std::cout << "Message received over UDP: " << buffer_udp << std::endl;
    return 1;
}



int main() {
    int tcp_socket = 0; // File descriptor for the socket
    char buffer_tcp[BUFFER_SIZE_TCP] = {0}; // Buffer to store messages

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

    // CREATING UDP SOCKET

    char buffer_udp[BUFFER_SIZE_TCP] = {0}; // Buffer to store messages
    int udpSocketR = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocketR == -1) {
        std::cerr << "Failed to create UDP socket" << std::endl;
        return -1;
    }

    // Bind the UDP socket to an address and port
    struct sockaddr_in serverAddressUDP_R;
    serverAddressUDP_R.sin_family = AF_INET;
    serverAddressUDP_R.sin_addr.s_addr = INADDR_ANY; // Listen on all available network interfaces
    serverAddressUDP_R.sin_port = htons(PORT_UDP_R);      // Replace with your desired port number
    socklen_t serverAddressUDP_Rlen = sizeof(serverAddressUDP_R);

    if (bind(udpSocketR, (struct sockaddr*)&serverAddressUDP_R, sizeof(serverAddressUDP_R)) == -1) {
        std::cerr << "Failed to bind UDP socket" << std::endl;
        return -1;
    }

 

    // Create a UDP socket to send
    int udpSocketS = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocketS == -1) {
        std::cerr << "Failed to create UDP socket" << std::endl;
        return -1;
    }

    // Server address and port
    struct sockaddr_in serverAddressUDP_S;
    serverAddressUDP_S.sin_family = AF_INET;
    serverAddressUDP_S.sin_port = htons(PORT_UDP_S);  // Replace with the server's port number
    serverAddressUDP_S.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address
    socklen_t serverAddressUDP_Slen = sizeof(serverAddressUDP_S);

    struct timeval timeout;
    timeout.tv_sec = 5; // Set a timeout of 5 seconds
    timeout.tv_usec = 0;
    setsockopt(udpSocketR, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    //Initiating the needed data structures
    int choice;
    string message1;
    string message2;
    
    while (true){

        
        //Printing out the main menu
        cout << endl;
        cout << "Main Menu - Please Input The Corresponding Integer" << endl;
        cout << "1.Split Text (Basic Encoding)" << endl;
        cout << "2.Merge Text (Basic Encoding)" << endl;
        cout << "3.Split Text (Advanced Encoding)" << endl;
        cout << "4.Merge Text (Advanced Encoding)" << endl;
        cout << "5.Quit" << endl << endl;

        cin.clear();
        
        cin >> choice;

        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        //Menu options
        switch (choice){

            //Split Text Basic Encoding    
            case 1:

                sendMessage(tcp_socket, "1");
                receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                if(buffer_tcp[0] == 'O' && buffer_tcp[1] == 'K' ){

                    buffer_tcp[0] = 0;
                    buffer_tcp[1] = 0;

                    cout << endl;


                    cout<< "-- Split Text (Basic Encoding) --" << endl;

                    cout << "Enter the text to split: " << endl;
                    cin >> ws;  
                    getline(cin, message1);
                    sendMessage(tcp_socket, message1.c_str());
                    receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                    receieveMessageUDP(udpSocketR, buffer_udp, BUFFER_SIZE_TCP, serverAddressUDP_R, serverAddressUDP_Rlen);

                    cout << endl;

                }else{
                    cout << "Something went wrong" << endl;
                }

                

                break;

            //Merge Text Basic Encoding
            case 2:

                sendMessage(tcp_socket, "2");
                receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                if(buffer_tcp[0] == 'O' && buffer_tcp[1] == 'K' ){

                    buffer_tcp[0] = 0;
                    buffer_tcp[1] = 0;

                    cout << endl;
                    cout << "-- Merge Text (Basic Encoding) --" << endl;
                    cout << "Enter the first text to merge: " << endl;
                    cin >> ws;  
                    getline(cin, message1);

                    cout << "Enter the second text to merge: " << endl;
                    getline(cin, message2);

                    sendMessage(tcp_socket, message1.c_str());
                    sendMessageUDP(udpSocketS, message2.c_str(), serverAddressUDP_S);
                    
                    receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                } else{
                    cout << "something went wrong" << endl;
                }

                break;

            //Split Text Advanced Encoding
            case 3:

                sendMessage(tcp_socket, "3");
                receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                if(buffer_tcp[0] == 'O' && buffer_tcp[1] == 'K' ){

                    buffer_tcp[0] = 0;
                    buffer_tcp[1] = 0;

                    cout << endl;

                    cout<< "-- Split Text (Advanced Encoding) --" << endl;

                    cout << "Enter the text to split: " << endl;
                    cin >> ws;  
                    getline(cin, message1);

                    sendMessage(tcp_socket, message1.c_str());

                    receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                    receieveMessageUDP(udpSocketR, buffer_udp, BUFFER_SIZE_TCP, serverAddressUDP_R, serverAddressUDP_Rlen);

                    cout << endl;
                }else{
                    cout << "something went wrong" << endl;
                }

                break;

            //Merge Text Advanced Encoding
            case 4:

                sendMessage(tcp_socket, "4");
                receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                if(buffer_tcp[0] == 'O' && buffer_tcp[1] == 'K' ){

                    buffer_tcp[0] = 0;
                    buffer_tcp[1] = 0;

                    cout << endl;
                    cout << "-- Merge Text (Advanced Encoding) --" << endl;
                    cout << "Enter the first text to merge: " << endl;
                    cin >> ws;  
                    getline(cin, message1);

                    cout << "Enter the second text to merge: " << endl;
                    getline(cin, message2);

                    sendMessage(tcp_socket, message1.c_str());
                    sendMessageUDP(udpSocketS, message2.c_str(), serverAddressUDP_S);
                    
                    receieveMessage(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
                }else{
                    cout << "something went wrong" << endl;
                }

                break;

            //Quitting the program
            case 5:

                cout << "Quitting" << endl;
                break;

            //Wrong inputs
            default:
                cout << "Please input a valid integer between 1-5" << endl;
                break;
                

        }
        //Quitting
        if (choice == 5){
            break;
        }

    }


    
    return 0;
}