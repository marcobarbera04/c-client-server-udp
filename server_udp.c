#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 128

int main()
{
    int server_socket_fd;                                   // server socket file descriptor
    struct sockaddr_in server_address;                      // server address (internet socket)
    struct sockaddr_in client_address;                      // client address (internet socket)
    int server_address_lenght = sizeof(server_address);     // size in bytes of the server socket
    int client_address_lenght = sizeof(client_address);     // size in bytes of the client socket

    char buffer[BUFFER_SIZE] = {0};

    char *server_message = "Message sent from server";

    // create the socket
    if((server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        perror("Could not create the socket");
        exit(EXIT_FAILURE);
    }

    // set up the actual address values
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // binding the socket with the address
    int isBind = bind(server_socket_fd, (struct sockaddr *)&server_address, server_address_lenght);
    if(isBind < 0){
        perror("Could not bind the socket to the address");
        close(server_socket_fd);
        exit(EXIT_FAILURE);
    }
    
    // udp server can't listen or accept for connection because is not connection oriented
    printf("Server is listening on port: %d\n", PORT);
    while(true)
    {
        ssize_t received_lenght = recvfrom(server_socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_address_lenght);
        if(received_lenght < 0)
        {
            perror("Error in receiving message");
            continue;       // continue listening for other messages
        }

        buffer[received_lenght] = '\n';     // terminate the string received
        printf("[Client]: %s", buffer);     // print the string received

        // send response to client
        ssize_t sent_lenght = sendto(server_socket_fd, server_message, strlen(server_message), 0, (struct sockaddr *)&client_address, client_address_lenght);
        if(sent_lenght < 0)
        {
            perror("Could not send response to client");
            continue;       // continue listening for other messages
        }
        printf("Message sent to client\n");
    }
    
    close(server_socket_fd);
    return 0;
}