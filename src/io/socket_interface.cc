#include "socket_interface.hpp"

#include <chrono>
#include <thread>

namespace Io
{
    void Server_socket_interface::lisent_connection() {
        listen(server_socket_fd, 5);
        socklen_t clilen = sizeof(cli_addr);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&cli_addr, &clilen);
        if (client_socket_fd < 0) {
            LOG_ERR("ERROR on accept message\n");
        }
    }
    void Server_socket_interface::task() {
        lisent_connection();
        int i = 0;
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int n = read(client_socket_fd, buffer, 256);
            if (n < 0) {
                LOG_ERR("ERROR reading from socket");
                lisent_connection();
            }
            LOG_OK(" %d Client message: %s\n", i++, buffer);
            n = write(client_socket_fd, "Server got your message", 24);
            if (n < 0) {
                lisent_connection();
                LOG_ERR("ERROR writing to socket");
            }

            unpack();
        }
    }

    inline void Server_socket_interface::unpack() {
        for (size_t i = 0; i < sizeof(Robot::Robot_set); i++) {
            *((uint8_t *)p_robot_set.get() + i) = buffer[i];
        }
        p_robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;
    }

    Server_socket_interface::Server_socket_interface(std::shared_ptr<Robot::Robot_set> robot_set)
        : port_num(51717),
          p_robot_set(robot_set) {
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket_fd < 0) {
            LOG_ERR("can't open socket\n");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port_num);

        if (bind(server_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            LOG_ERR("can't bind socket fd with port number");
        }
    }

    Server_socket_interface::~Server_socket_interface() {
        close(client_socket_fd);
        close(server_socket_fd);
    }
}  // namespace Io
