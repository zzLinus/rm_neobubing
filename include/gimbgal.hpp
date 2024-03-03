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
        void init();
        void control_loop();
        void unpack(const can_frame &frame);
        void update_speed();
        void send_motor_current();

       public:
        bool no_force = true;
        fp32 v_yaw_set = 0.f;
        fp32 yaw = 0.f;
        fp32 yaw_set = 0.f;

        std::shared_ptr<Hardware::Can_interface> can_itrf;
        Hardware::Motor yaw_motor;
        Hardware::Motor pitch_motor;
        Pid::Pid_rad yaw_absolute_pid;
//        Pid::Pid_rad pitch_absolute_pid;
    };

}  // namespace Gimbal
#endif
