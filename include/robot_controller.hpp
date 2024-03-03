#pragma once
#include "config.hpp"
#include "robot.hpp"
#include "chassis.hpp"
#include "gimbgal.hpp"

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
        void gimbal_task();

       public:

        std::unique_ptr<std::thread> chassis_tread;
        std::unique_ptr<std::thread> chassis_can_tread;
        std::unique_ptr<std::thread> gimbal_tread;
        std::unique_ptr<std::thread> gimbal_can_tread;

        Pid::Pid_rad chassis_angle_pid;
        std::shared_ptr<Robot_set> robot_set;
        Chassis::Chassis chassis;
        Gimbal::Gimbal gimbal;
        bool start();
       private:
    };
}
