#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_sockets.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main (int argc, char **argv)
{

    if (argc != 2) error("not enough arguments");

    int port_number = atoi(*(argv + 1));
    int keep_alive = 1;

    char stdin_buffer[256] = {};

    SocketObject client;

    socket_common_init(&client);

    socket_common_config(&client, socket_type_client, AF_INET, INADDR_ANY, htons(port_number));

    if (socket_common_fd(&client, SOCK_STREAM) < 0) error("failed to get file descriptor\r\n");

    if (socket_client_connect(&client) < 0) error("failed to connect\r\n");

    printf("Connected to server\r\n");

    while (keep_alive > 0)
    {

        fgets(stdin_buffer, 256, stdin); // blocking
        // printf("entered: %s", stdin_buffer);

        if(!strncmp(stdin_buffer, "q", 1))
        {
            keep_alive = 0;
        } 
        else
        {
            if (socket_client_write(&client, stdin_buffer, strlen(stdin_buffer) + 1) < 0) keep_alive = 0;
        } 

    }

    printf("Closing connection\r\n");

    socket_common_close(&client);

    return 0;

}