// Created by rhine on 2/28/24.
//
#include "chassis_ctrl.hpp"

namespace Chassis {

    void Chassis_ctrl::task() {
        while(true) {
            if(robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                chassis.no_force = true;
            }
            else if(robot_set->mode == Types::ROBOT_MODE::ROBOT_NOT_FOLLOW) {
                chassis.no_force = false;
                chassis.vx_set = robot_set->vx_set;
                chassis.vy_set = robot_set->vy_set;
            }
            else if(robot_set->mode == Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL) {
                fp32 sin_yaw = std::sin(-robot_set->chassis_relative_angle);
                fp32 cos_yaw = std::cos(-robot_set->chassis_relative_angle);
                chassis.vx_set = cos_yaw * robot_set->vx_set + sin_yaw * robot_set->vy_set;
                chassis.vy_set = -sin_yaw * robot_set->vx_set + cos_yaw * robot_set->vy_set;
                chassis_angle_pid.calc(robot_set->chassis_relative_angle, 0.f);
                chassis.wz_set = chassis_angle_pid.out;
            }
        }
    }

    Chassis_ctrl::Chassis_ctrl(const std::shared_ptr<Robot::Robot_set> &robot) :
          chassis_angle_pid(Config::CHASSIS_FOLLOW_GIMBAL_PID_CONFIG) {
        robot_set = robot;
    }
}


