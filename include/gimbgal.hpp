#ifndef __GIMBAL__
#define __GIMBAL__

#include <memory>

#include "motor.hpp"
#include "can.hpp"
#include "robot.hpp"
#include "config.hpp"
#include "robot.hpp"

namespace Gimbal
{

    class Gimbal
    {
       public:
        Gimbal();
        ~Gimbal() = default;
        void init(const std::shared_ptr<Robot::Robot_set>& robot);
        void control_loop();
        void unpack(const can_frame &frame);
        void update_data();
        void send_motor_current();

       public:
        bool no_force = true;
        fp32 yaw_set = 0.f;
        fp32 pitch_set = 0.f;

        std::shared_ptr<Hardware::Can_interface> can_itrf;
        std::shared_ptr<Robot::Robot_set> robot_set;
        Hardware::Motor yaw_motor;
        Hardware::Motor pitch_motor;
        Pid::Pid_rad yaw_absolute_pid;
        Pid::Pid_rad pitch_absolute_pid;
    };

}  // namespace Gimbal
#endif
