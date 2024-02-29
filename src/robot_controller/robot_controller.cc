#include "robot_controller.hpp"
#include "can.hpp"

namespace Robot
{
    Robot_ctrl::Robot_ctrl() :
          chassis_angle_pid(Config::M3508_SPEED_PID_CONFIG) {
        robot_set = std::make_shared<Robot_set>();
        chassis.init();
    }

    bool Robot_ctrl::start() {
        auto f = &Hardware::Can_interface::can_dump;
        chassis_can_tread = std::make_unique<std::thread>(
            &Hardware::Can_interface::can_dump,
            chassis.can_itrf
            );
        chassis_tread = std::make_unique<std::thread>(
            &Robot_ctrl::chassis_task,
            this
            );
        return true;
    }

    void Robot_ctrl::chassis_task() {
        while(true) {
            if(robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                chassis.no_force = true;
            }
            else {
                chassis.no_force = false;

                fp32 sin_yaw, cos_yaw;
                sincosf(-robot_set->chassis_relative_angle, &sin_yaw, &cos_yaw);
                chassis.vx_set = cos_yaw * robot_set->vx_set + sin_yaw * robot_set->vy_set;
                chassis.vy_set = -sin_yaw * robot_set->vx_set + cos_yaw * robot_set->vy_set;

                if(robot_set->mode == Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL) {
                    chassis_angle_pid.calc(robot_set->chassis_relative_angle, 0.f);
                    chassis.wz_set = chassis_angle_pid.out;
                }
                else {
                    chassis.wz_set = robot_set->wz_set;
                }
            }
            chassis.control_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
};