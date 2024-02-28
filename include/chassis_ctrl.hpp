#pragma once
#include <cmath>

#include "chassis.hpp"
#include "robot.hpp"
namespace Chassis
{
    class Chassis_ctrl
    {
       public:
        Chassis_ctrl() = default;
        explicit Chassis_ctrl(const std::shared_ptr<typename Robot::Robot_set> &robot);
        void task();

       public:
        std::shared_ptr<Robot::Robot_set> robot_set;
        Chassis chassis;
        Pid::Pid_ctrl chassis_angle_pid;
    };
}