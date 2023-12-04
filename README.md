# CPSC441-ClientServerSocketMessaging
 
Name: Bartu Okan

This application has one server and one client side C++ source code to be run simultaneously.
The server needs to be run first, then the client.

Client-side handles user-input, menu navigation and displaying results from server.
Server-side encodes or decodes the received input from the client-side and sends it back to the client.
They both use a combination of TCP and UDP depending on the chosen option.

To run the client code: 
//To Compile: g++ -o client client.cpp
//To Run: ./client

To run the server code:
//To Compile: g++ -o server server.cpp
//To Run: ./server

This application was tested on Kali Linux Virtual Machine ran using Oracle VM VirtualBox.

On client-side, for option 2.Merge Message (Basic Encoding), it is expected that the user inputs strings of same length including whitespaces.
On client-side, for option 4.Merge Message (Advanced Encoding), it is expected that the user inputs valid strings for the merging algorithm.

If you wanted to run the applications with different configurations, you can change the port numbers in the codes, or change the IP address on the client
code if you want to run over different computers.

