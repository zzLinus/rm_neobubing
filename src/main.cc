#include <iostream>

#include "iostream"
#include "robot_controller.hpp"
#include "thread"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;

    robot.load_hardware();
    robot.start_init();
    robot.init_join();

    robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;

    robot.start();

    robot.join();

    return 0;
}
