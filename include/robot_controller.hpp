#pragma once
#include "config.hpp"
#include "robot.hpp"
#include "chassis.hpp"

#include <thread>
#include <memory>

namespace Robot
{

    class Robot_ctrl
    {
       public:

        Robot_ctrl();
        ~Robot_ctrl() = default;
        void chassis_task();

       public:

        std::unique_ptr<std::thread> chassis_tread;
        std::unique_ptr<std::thread> chassis_can_tread;

        Pid::Pid_position chassis_angle_pid;
        std::shared_ptr<Robot_set> robot_set;
        Chassis::Chassis chassis;
        bool start();
       private:
    };
}