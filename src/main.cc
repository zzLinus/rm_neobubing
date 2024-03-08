#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "iostream"
//#include "types.hpp"
//#include "utils.hpp"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    Io::Server_socket_interface socket_intrf(robot.robot_set);
    std::thread socket_thread(&Io::Server_socket_interface::task, &socket_intrf);

    try {
        robot.load_hardware();
    } catch (serial::IOException) {
        LOG_ERR("there's no such serial device\n");
    }
    robot.start();

    robot.join();
//    socket_thread.join();
    // robot.start_init();
    // robot.init_join();

    return 0;
}
