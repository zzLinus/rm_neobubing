#include "robot_controller.hpp"

namespace Robot
{
    Robot_ctrl::Robot_ctrl() : chassis_angle_pid(Config::CHASSIS_FOLLOW_GIMBAL_PID_CONFIG) {
        robot_set = std::make_shared<Robot_set>();
    }

    Robot_ctrl::~Robot_ctrl() {
        delete ser1;
    }

    void Robot_ctrl::start_init() {
        chassis.init(robot_set);
        gimbal.init(robot_set);
        shoot.init(robot_set);
        gimbal_init_thread = std::make_unique<std::thread>(&Robot_ctrl::gimbal_init_task, this);
    }

    void Robot_ctrl::init_join() const {
        if (gimbal_init_thread != nullptr) {
            gimbal_init_thread->join();
        }
    }

    void Robot_ctrl::start() {
        // #warning chassis is closed
        chassis_thread = std::make_unique<std::thread>(&Robot_ctrl::chassis_task, this);
        gimbal_thread = std::make_unique<std::thread>(&Robot_ctrl::gimbal_task, this);
        shoot_thread = std::make_unique<std::thread>(&Robot_ctrl::shoot_task, this);
    }

    void Robot_ctrl::join() const {
        if (hardware != nullptr) {
            hardware->join();
        }
        if (chassis_thread != nullptr) {
            chassis_thread->join();
        }
        if (gimbal_thread != nullptr) {
            gimbal_thread->join();
        }
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
        gimbal.start_init_loop();
        while (!gimbal.inited) {
            gimbal.init_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void Robot_ctrl::shoot_task() {
        while (true) {
            if (robot_set->mode == Types::ROBOT_MODE::ROBOT_NO_FORCE) {
                shoot.no_force = true;
            } else {
                shoot.no_force = false;
                shoot.friction_open = robot_set->friction_open;
                shoot.shoot_open = robot_set->shoot_open;
            }
            shoot.control_loop();
            std::this_thread::sleep_for(std::chrono::milliseconds(Config::SHOOT_CONTROL_TIME));
        }
    }

    void Robot_ctrl::load_hardware() {
        can0.init("can0");
        can1.init("can1");
        socket_intrf = new Io::Server_socket_interface();
        try {
            ser1 = new Hardware::Serial_interface<Types::ReceivePacket>("/dev/ttyACM0", 115200, 1000);
        } catch (serial::IOException &ex) {
            LOG_ERR("there's no such serial device\n");
        }

        hardware = std::make_shared<RobotHardware>(can0, can1, *ser1, *socket_intrf, rc_ctrl);

        Robot::hardware->register_callback<SOCKET, Robot::Vison_control>([&](const Robot::Vison_control &vc) {
            robot_set->vx_set = vc.linear_vx;
            robot_set->vy_set = vc.linear_vy;
            robot_set->wz_set = vc.angular;
            robot_set->yaw_set = vc.yaw_set;
        });

        Robot::hardware->register_callback<SOCKET, Robot::ReceiveGimbalPacket>([&](const auto &pkg) {
            LOG_ERR("get receive gimbal packet\n");
            LOG_ERR("x: %f, y: %f, z: %f\n", pkg.x, pkg.y, pkg.z);
        });

        Robot::hardware->register_callback<RC_CTRL>([&](Io::Rc_ctrl *rc) {
            // LOG_INFO("rc ctrl : %d %d\n", rc->vx, rc->vy);
            //  robot_set->vx_set = rc->vx;
            //  robot_set->vy_set = rc->vy;
        });

        Robot::hardware->register_callback<SER1>([&](const Types::ReceivePacket &rp) {
            robot_set->ins_yaw = rp.yaw;
            robot_set->ins_pitch = rp.pitch;
            robot_set->ins_roll = rp.roll;
            robot_set->ins_yaw_v = rp.yaw_v;
            robot_set->ins_pitch_v = -rp.pitch_v;
            robot_set->ins_roll_v = -rp.roll_v;
            rc_ctrl.vx = rp.ch[RC_X_CHANNEL];
            rc_ctrl.vy = rp.ch[RC_Y_CHANNEL];
            rc_ctrl.pitch = rp.ch[RC_PITCH_CHANNEL];
            rc_ctrl.yaw = rp.ch[RC_YAW_CHANNEL];
            rc_ctrl.bullet_bag = rp.ch[RC_BULLET_BAG_CHANNEL];
            rc_ctrl.switch1_state = rp.s[0];
            rc_ctrl.switch2_state = rp.s[1];
            rc_ctrl.send_control();

            Robot::SendGimbalPacket pkg;
            pkg.yaw = rp.yaw;
            pkg.pitch = rp.pitch;
            pkg.roll = rp.roll;
            Robot::hardware->send<SOCKET>(pkg);
            //            LOG_INFO("send pkg\n");
        });
    }
};  // namespace Robot
