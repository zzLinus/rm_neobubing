#include <iostream>

#include "io.hpp"
#include "thread"
#include "robot_controller.hpp"

#include <memory>

void run(Robot::Robot_ctrl * robot) {
    fp32 speed = 0.f, df = 0.0005f;
    robot->robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;
    while(true) {
        if(fabs(speed) >= 2.0) {
            df *= -1.f;
        }
        speed += df;
        robot->robot_set->vx_set = speed;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

Robot::Robot_ctrl robot;
int main() {
    robot.start();
    std::thread main_thread(run, &robot);
    main_thread.join();
    return 0;
}
