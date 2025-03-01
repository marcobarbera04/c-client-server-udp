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
    int client_socket_fd;
    struct sockaddr_in server_address;
    int server_address_lenght = sizeof(server_address);
    char *client_message = "Message sent from client";
    char buffer[BUFFER_SIZE] = {0};

    // create the socket
    if((client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Could not create the socket");
        exit(EXIT_FAILURE);
    }

    // set the server address actual values
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // sending the message to the server
    ssize_t sent_lenght = sendto(client_socket_fd, client_message, strlen(client_message), 0, (struct sockaddr *)&server_address, server_address_lenght);
    if(sent_lenght < 0)
    {
        perror("Could not send the message");
        close(client_socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Message sent to the server\n");

    // receiving message from the server
    ssize_t received_lenght = recvfrom(client_socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &server_address_lenght);
    if(received_lenght < 0)
    {
        perror("Could not receive message");
        close(client_socket_fd);
        exit(EXIT_FAILURE);
    }

    buffer[received_lenght] = '\n';     // terminate the string received
    printf("[Server]: %s", buffer);     // print the string received

    close(client_socket_fd);
    return 0;
}