#include "robot_controller.hpp"

#include "can.hpp"

namespace Robot
{
    Robot_ctrl::Robot_ctrl() : chassis_angle_pid(Config::CHASSIS_FOLLOW_GIMBAL_PID_CONFIG) {
        robot_set = std::make_shared<Robot_set>();
        chassis.init(robot_set);
        gimbal.init(robot_set);
    }

    void Robot_ctrl::start_init() {
        gimbal_can_thread = std::make_unique<std::thread>(&Hardware::Can_interface::can_dump, gimbal.can_itrf);
        gimbal_init_thread = std::make_unique<std::thread>(&Robot_ctrl::gimbal_init_task, this);
    }

    void Robot_ctrl::init_join() const {
        if (gimbal_init_thread != nullptr) {
            gimbal_init_thread->join();
        }
    }

    void Robot_ctrl::start() {
#warning chassis is closed
        chassis_can_thread = std::make_unique<std::thread>(&Hardware::Can_interface::can_dump, chassis.can_itrf);
        chassis_thread = std::make_unique<std::thread>(&Robot_ctrl::chassis_task, this);
        // gimbal_thread = std::make_unique<std::thread>(&Robot_ctrl::gimbal_task, this);
    }

    void Robot_ctrl::join() const {
        if (chassis_thread != nullptr) {
            chassis_thread->join();
        }
        // if(gimbal_thread != nullptr) {
        //     gimbal_thread->join();
        // }
    }

    void Robot_ctrl::chassis_task() {
        while (true) {
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                chassis.no_force = true;
            } else {
                chassis.no_force = false;

                fp32 sin_yaw, cos_yaw;
                sincosf(-robot_set->yaw_relative, &sin_yaw, &cos_yaw);
                chassis.vx_set = cos_yaw * robot_set->vx_set + sin_yaw * robot_set->vy_set;
                chassis.vy_set = -sin_yaw * robot_set->vx_set + cos_yaw * robot_set->vy_set;

                if (robot_set->mode == Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL) {
                    chassis_angle_pid.calc(robot_set->yaw_relative, 0.f);
                    chassis.wz_set = chassis_angle_pid.out;
                } else {
                    chassis.wz_set = robot_set->wz_set;
                }
            }
            chassis.control_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    void Robot_ctrl::gimbal_init_task() {
        while (!gimbal.inited) {
            gimbal.init_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }

    void Robot_ctrl::gimbal_task() {
        while (true) {
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                gimbal.no_force = true;
            } else {
                gimbal.no_force = false;
                gimbal.yaw_set = robot_set->yaw_set;
                gimbal.pitch_set = robot_set->pitch_set;
            }
            gimbal.control_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }
    }
};  // namespace Robot
