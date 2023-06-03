#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main()
{
    // create a socket
    int soc = socket(AF_INET, SOCK_DGRAM, 0);
    if (soc == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    int enable_broadcast;
    u_int32_t optlen;
    if (getsockopt(soc, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, &optlen) == -1)
    {
        std::cerr << "Failed to query broadcasting on socket." << std::endl;
        close(soc);
        return 1;
    }
    printf("Broadcasting status: %d \n", enable_broadcast);
    assert(enable_broadcast == false);
    // enable broadcasting on the socket
    enable_broadcast = 1;
    if (setsockopt(soc, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, sizeof(enable_broadcast)) == -1)
    {
        std::cerr << "Failed to enable broadcasting on socket." << std::endl;
        close(soc);
        return 1;
    }
    printf("enable broadcasting \n");

    if (getsockopt(soc, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, &optlen) == -1)
    {
        std::cerr << "Failed to query broadcasting on socket." << std::endl;
        close(soc);
        return 1;
    }
    printf("Broadcasting status: %d \n", enable_broadcast);
    // assert(enable_broadcast == true);

    // send a broadcast message
    struct sockaddr_in broadcast_address;
    broadcast_address.sin_family = AF_INET;
    broadcast_address.sin_addr.s_addr = INADDR_BROADCAST;
    broadcast_address.sin_port = htons(8080); // broadcast to port 8080
    const char *message = "Hello, world!";
    if (sendto(soc, message, strlen(message), 0, (struct sockaddr *)&broadcast_address, sizeof(broadcast_address)) == -1)
    {
        std::cerr << "Failed to send broadcast message." << std::endl;
        close(soc);
        return 1;
    }

    // close the socket
    close(soc);
    printf("done\n");
    return 0;
}