#pragma once
#include "config.hpp"
#include "chassis_ctrl.hpp"

#include <thread>
#include <memory>

namespace Robot
{

    class Robot_ctrl
    {
       public:
        std::unique_ptr<std::thread> chassis_tread;
        std::unique_ptr<std::thread> chassis_can_tread;
       public:

        Robot_ctrl();
        ~Robot_ctrl() = default;
        std::shared_ptr<const Robot_set> get_robot_read() const;
        std::shared_ptr<Robot_set> get_robot_write();
        std::shared_ptr<Robot_set> robot_set;
        Chassis::Chassis_ctrl chassis_ctrl;
        bool start();
       private:
    };
}