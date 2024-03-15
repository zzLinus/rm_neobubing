#ifndef __SOCKET_INTERFACE__
#define __SOCKET_INTERFACE__

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <memory>

#include "arpa/inet.h"
#include "robot.hpp"
#include "utils.hpp"

namespace Io
{
    class Server_socket_interface : public Hardware::Callback<Robot::Vison_control, Robot::ReceiveGimbalPacket>

    {
       public:
        Server_socket_interface();
        ~Server_socket_interface();
        void task();

        template<typename T>
        void send(const T &pkg) {
            auto n = sendto(
                sockfd,
                (const char *)(&pkg),
                sizeof(pkg),
                MSG_CONFIRM,
                (const struct sockaddr *)&cli_addr,
                sizeof(cli_addr));

            //            LOG_OK("%lu bytes send to client\n", n);
        }

        void send(const std::string &str) {
            auto n =
                sendto(sockfd, str.c_str(), str.size(), MSG_CONFIRM, (const struct sockaddr *)&cli_addr, cli_addr_len);
            //            LOG_OK("%lu bytes send to client\n", n);
        }

        void send(const char *str) {
            auto n = sendto(sockfd, str, strlen(str), MSG_CONFIRM, (const struct sockaddr *)&cli_addr, cli_addr_len);
            //            LOG_OK("%lu bytes send to client\n", n);
        }

       private:
        int64_t port_num;
        int sockfd;
        socklen_t cli_addr_len;
        sockaddr_in serv_addr, cli_addr;
        char buffer[256];

       public:
       private:
    };
}  // namespace Io

#endif
