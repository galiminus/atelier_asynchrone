#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdlib.h>

int
main ()
{
    int                         server_fd;
    struct sockaddr_in          address;

    int                         client_fd;

    char                        buffer[2048];
    int                         read_len;

    /**
     * Create a TCP/IPv4 socket
     */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (int []){1}, sizeof(int));

    /**
     * Bind the socket to all interfaces
     */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(10000);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    /**
     * Make it listen for incoming connections
     */
    listen(server_fd, 1024);

    while (true) {
        client_fd = accept(server_fd, NULL, NULL);
        write(client_fd, "Hello - Echo Server\n", 20);

        do {
            read_len = read(client_fd, buffer, sizeof(buffer));
            if (read_len == 0) {
                break ;
            }

            write(client_fd, buffer, read_len);
        } while (read_len != 5 || strncmp(buffer, "QUIT\n", 5) != 0);

        close(client_fd);
    }
}
