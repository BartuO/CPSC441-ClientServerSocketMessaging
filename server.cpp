#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT_TCP 8080 
#define PORT_UDP_S 8092
#define PORT_UDP_R 8091

#define BUFFER_SIZE_TCP 6144

using namespace std;

//Name: Bartu Okan
//UCID: 30150180
//Tutorial: T04

//To Compile: g++ -o server server.cpp
//To Run: ./server



//Function to send data over TCP
void sendMessageTCP(int tcp_socket, const char* message){
    // Sending a message to the server
    send(tcp_socket, message, strlen(message), 0);
    std::cout << "Message sent over TCP:" << message << std::endl;
}

//Function to receive data over TCP
void receieveMessageTCP(int tcp_socket, char* buffer_tcp, int bufferSize){
    int bytesRead = read(tcp_socket, buffer_tcp, bufferSize - 1);
    if (bytesRead < 0) {
        std::cerr << "Error reading from server" << std::endl;
    } else {
        buffer_tcp[bytesRead] = '\0'; // Null-terminate the received data
        std::cout << "Message received over TCP: " << buffer_tcp << std::endl;
    }
}

//Function to send data over UDP
int sendMessageUDP(int udp_socket, const char* message, struct sockaddr_in& address){
    int bytesSent = sendto(udp_socket, message, strlen(message), 0, (struct sockaddr*)&address, sizeof(address));
    if (bytesSent == -1) {
        std::cerr << "Failed to send data to the server" << std::endl;
        return -1;
    }
    std::cout << "Message sent over UDP: " << message << std::endl;
    return 1;
   
}

//Function to receieve data over UDP
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

//Does the basic splitting operation
void splitBasic(const char* input, string& consonants, string& vowels){

    for (size_t i = 0; input[i] != '\0'; i++) {
        if (strchr("AEIOUaeiou", input[i]) != NULL) {
            vowels += input[i];
            consonants += ' ';
        } else {
            vowels += ' ';
            consonants += input[i];
        }
    }

}

//Does the basic merging operation
void mergeBasic(const char* input1, const char* input2, string& mergedString){

    for (size_t i = 0; input1[i] != '\0'; i++){
        if (input1[i] == ' '){
            mergedString += input2[i];
        } else{
            mergedString += input1[i];
        }
    }
}

//Does the advanced split operation
void splitAdvanced(const char* input, string& consonants, string& vowels){


    int space_since_last_vowel = 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (strchr("AEIOUaeiou", input[i]) != NULL) {
            vowels += to_string(space_since_last_vowel);
            vowels += input[i];
            space_since_last_vowel = 0;
        } else {
            space_since_last_vowel++;
            consonants += input[i];
        }
    }
    
}

//Does the advanced merge operation
void mergeAdvanced(const char* consonants, const char* vowels, string& mergedString){

    int consonant_index = 0;
    int consonant_increment = 0;


    for (int i = 0; vowels[i] != '\0'; i += 2){

        int skipFor = vowels[i] - '0';


        
        for (int b = consonant_index; b < (skipFor + consonant_index); b++ ){

      

            mergedString += consonants[b];
            consonant_increment++;
           

            
        }
        consonant_index += consonant_increment;
        consonant_increment = 0;
        mergedString += vowels[i + 1];
        


    } 

    if(consonant_index < strlen(consonants)){
        for (int i = consonant_index; i < strlen(consonants); i++){
            mergedString += consonants[i];
        }
    }

    

}


int main() {

    //CREATING TCP SOCKET

    int server_fd, tcp_socket; // File descriptors for the server and the new client socket
    struct sockaddr_in tcp_address; // Structure containing an internet address
    int tcp_addrlen = sizeof(tcp_address);
    char buffer_tcp[BUFFER_SIZE_TCP] = {0}; // Buffer to store messages

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
    if ((tcp_socket = accept(server_fd, (struct sockaddr *)&tcp_address, (socklen_t *)&tcp_addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
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


    //Timeout for UDP packet loss
    struct timeval timeout;
    timeout.tv_sec = 5; // Set a timeout of 5 seconds
    timeout.tv_usec = 0;
    setsockopt(udpSocketR, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));


    if (bind(udpSocketR, (struct sockaddr*)&serverAddressUDP_R, serverAddressUDP_Rlen) == -1) {
        std::cerr << "Failed to bind UDP socket" << std::endl;
        return -1;
    }

    std::cout << "UDP Server is listening for incoming datagrams..." << std::endl;

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

    

    //Initial buffer to determine which operation
    char initial_buffer[5] = {0}; 


    while (true){

        receieveMessageTCP(tcp_socket, initial_buffer, 5);

        //After determining the operation, it will execute it

        //Basic Split Case
        if(initial_buffer[0] == '1'){
            initial_buffer[0] = 0;

            sendMessageTCP(tcp_socket, "OK");
            receieveMessageTCP(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);


            string consonants, vowels;

            splitBasic(buffer_tcp, consonants, vowels);


            sendMessageTCP(tcp_socket, consonants.c_str());
            sendMessageUDP(udpSocketS, vowels.c_str(), serverAddressUDP_S);

        }

        //Basic Merge Case
        else if (initial_buffer[0] == '2'){
            initial_buffer[0] = 0;
            sendMessageTCP(tcp_socket, "OK");
            receieveMessageTCP(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
            receieveMessageUDP(udpSocketR, buffer_udp, BUFFER_SIZE_TCP, serverAddressUDP_R, serverAddressUDP_Rlen);


            
            string mergedString;

  

            mergeBasic(buffer_tcp, buffer_udp, mergedString);



            sendMessageTCP(tcp_socket, mergedString.c_str()); 

    
        }

        //Advanced Split Case
        else if (initial_buffer[0] == '3'){
            initial_buffer[0] = 0;
            sendMessageTCP(tcp_socket, "OK");
            receieveMessageTCP(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);

            string consonants, vowels;

            splitAdvanced(buffer_tcp, consonants, vowels);


            sendMessageTCP(tcp_socket, consonants.c_str());
            sendMessageUDP(udpSocketS, vowels.c_str(), serverAddressUDP_S);
        }

        //Advanced Merge Case
        else if (initial_buffer[0] == '4'){
            initial_buffer[0] = 0;
            sendMessageTCP(tcp_socket, "OK");

            receieveMessageTCP(tcp_socket, buffer_tcp, BUFFER_SIZE_TCP);
            receieveMessageUDP(udpSocketR, buffer_udp, BUFFER_SIZE_TCP, serverAddressUDP_R, serverAddressUDP_Rlen);

         

            
            string mergedString;


            mergeAdvanced(buffer_tcp, buffer_udp, mergedString);



            sendMessageTCP(tcp_socket, mergedString.c_str());


        }else{
            //Will send "NOTOK" if wrong message is received.
            sendMessageTCP(tcp_socket, "NOTOK");
        }


        


    }   


        



        

    
    

    return 0;
}

