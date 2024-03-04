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

        void start_init();
        void init_join() const;
        void start();
        void join() const;

        void chassis_task();
        void gimbal_task();
        void gimbal_init_task();

       public:

        std::unique_ptr<std::thread> chassis_thread;
        std::unique_ptr<std::thread> chassis_can_thread;
        std::unique_ptr<std::thread> gimbal_thread;
        std::unique_ptr<std::thread> gimbal_can_thread;
        std::unique_ptr<std::thread> gimbal_init_thread;

        Pid::Pid_rad chassis_angle_pid;
        std::shared_ptr<Robot_set> robot_set;
        Chassis::Chassis chassis;
        Gimbal::Gimbal gimbal;
       private:
    };
}
