#include <iostream>

#include "io.hpp"
#include "thread"
#include "robot_controller.hpp"

void test(Robot::Robot_ctrl * robot) {
    fp32 speed = 0.f, df = 0.001f;
    robot->robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;
    while(true) {
        if(fabs(speed) >= 15.0) {
            df *= -1.f;
        }
        speed += df;
        robot->robot_set->v_yaw_set = speed;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}

int main() {
    Robot::Robot_ctrl robot;
    robot.start();
    std::thread main_thread(test, &robot);
    main_thread.join();
    return 0;
}
