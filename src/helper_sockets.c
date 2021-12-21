#include "helper_sockets.h"

int socket_common_init(SocketObject *socket_obj)
{

    socket_obj->type = socket_type_unassigned;
    socket_obj->status = socket_status_init;

    socket_obj->socket_fd = -1;
    socket_obj->socket_port_no = 0;

    bzero(&socket_obj->socket_address, sizeof(socket_obj->socket_address));
    
    return 0;

}

int socket_common_config(SocketObject *socket_obj, SocketType type, uint16_t sin_family, uint16_t sin_addr, uint16_t sin_port)
{

    socket_obj->type = type;

    /* inet_pton(AF_INET, "192.168.1.xxx", &socket_obj.server_address); for ip addresses */
    socket_obj->socket_address.sin_family = sin_family;
    socket_obj->socket_address.sin_addr.s_addr = sin_addr;
    socket_obj->socket_address.sin_port = htons(sin_port); /* can't be less than 1024 if server */
    socket_obj->socket_address_length = sizeof(socket_obj->socket_address);

    socket_obj->status = socket_status_init;

    return 0;

}

int socket_common_fd(SocketObject *socket_obj, uint16_t socket_protocol_type)
{

    socket_obj->socket_fd = socket(socket_obj->socket_address.sin_family, socket_protocol_type, 0);

    if (socket_obj->socket_fd < 0)
    {
        return -1;
    }

    socket_obj->status = socket_status_fd_set;

    return 0;

}

int socket_server_bind(SocketObject *socket_obj, uint16_t num_connection_request)
{

    if(socket_obj->type == socket_type_client)
    {
        return -1;
    }
    else
    {

        int err = bind(socket_obj->socket_fd, (struct sockaddr *) &socket_obj->socket_address, socket_obj->socket_address_length);

        if(err < 0)
        {
            return -1;
        }

        err = listen(socket_obj->socket_fd, num_connection_request);

        if(err < 0)
        {
            return -1;
        }

    }

    socket_obj->status = socket_status_listen;

    return 0;

}

int socket_server_accept(SocketObject *socket_server_obj, SocketObject *socket_client_obj)
{

    /* clilen = sizeof(cli_addr) */
    /* accept(socket_obj.socket_fd, (struct sockaddr *) NULL, NULL) */
    socket_client_obj->socket_fd = accept(socket_server_obj->socket_fd, (struct sockaddr *) &socket_client_obj->socket_address, &socket_client_obj->socket_address_length);

    if (socket_client_obj->socket_fd < 0)
    {
        return -1;
    }

    socket_client_obj->type = socket_type_client;
    socket_client_obj->status = socket_status_accepted;

    return 0;

}

int socket_client_connect(SocketObject *socket_obj)
{

    int connect_status = connect(socket_obj->socket_fd, (struct sockaddr *) &socket_obj->socket_address, socket_obj->socket_address_length);

    if (connect_status < 0)
    {
        return -1;
    }

    socket_obj->status = socket_status_connected;

    return 0;

}

int socket_common_close(SocketObject *socket_obj)
{

    close(socket_obj->socket_fd);

    socket_obj->status = socket_status_closed;

    return 0;

}

int socket_client_write(SocketObject *socket_obj, char *string, uint16_t str_len)
{

    int write_status = write(socket_obj->socket_fd, string, str_len);

    if (write_status < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }

}

int socket_client_read(SocketObject *socket_obj, char *buffer, uint16_t buffer_size, uint16_t *str_len)
{

    *str_len = read(socket_obj->socket_fd, buffer, buffer_size - 1);

    if (*str_len < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }   

}