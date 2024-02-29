#ifndef __GIMBAL__
#define __GIMBAL__

#include <memory>

#include "motor.hpp"
#include "robot.hpp"
#include "config.hpp"

namespace Gimbal
{

    class Gimbal
    {
       public:
        Gimbal();
        ~Gimbal() = default;
        void init(const std::shared_ptr<Robot::Robot_set> &robot);
        void control_loop();

       public:
        Hardware::Motor<Pid::Pid_rad> yaw_motor;
        Hardware::Motor<Pid::Pid_rad> pitch_motor;

        std::shared_ptr<Robot::Robot_set> robot_set;
    };

}  // namespace Gimbal
#endif
