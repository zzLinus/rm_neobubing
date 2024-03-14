#include "shoot.hpp"

namespace Shoot
{

    Shoot::Shoot() : friction_ramp(Config::FRICTION_ADD_SPEED, Config::SHOOT_CONTROL_TIME * 1e-3f) {
        friction.assign(4, Hardware::Motor{ Config::FRIC_SPEED_PID_CONFIG });
        trigger.assign(2, Hardware::Motor{ Config::TRIGGER_SPEED_PID_CONFIG });
    }

    void Shoot::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;
        for (int i = 0; i < friction.size(); i++) {
            auto &mot = friction[i];
            Robot::hardware->register_callback<CAN0>(
                0x201 + i, [&mot](const auto &frame) { return mot.unpack(frame); });
        }
//        for (int i = 0; i < trigger.size(); i++) {
//            auto &mot = trigger[i];
//            Robot::hardware->register_callback<CAN0>(
//                0x205 + i, [&mot](const auto &frame) { return mot.unpack(frame); });
//        }
    }

    void Shoot::update_speed() {
        for (auto &m : friction) {
            m.speed = m.motor_measure.speed_rpm;
        }
        for (auto &m : trigger) {
            m.speed = m.motor_measure.speed_rpm;
        }
    }

    void Shoot::decomposition_speed() {
        if (no_force) {
            friction_ramp.clear();
        } else if (friction_open) {
            friction_ramp.update(Config::FRICTION_MAX_SPEED);
        } else {
            friction_ramp.update(0.f);
        }
        friction[0].speed_set = friction_ramp.out;
        friction[1].speed_set = -friction_ramp.out;
        friction[2].speed_set = friction_ramp.out;
        friction[3].speed_set = -friction_ramp.out;
    }

    void Shoot::send_motor_current() {
        can_frame frame{};
        frame.can_id = 0x200;
        frame.can_dlc = 8;
        frame.data[0] = (friction[0].give_current >> 8);
        frame.data[1] = (friction[0].give_current & 0xff);
        frame.data[2] = (friction[1].give_current >> 8);
        frame.data[3] = (friction[1].give_current & 0xff);
        Robot::hardware->send<CAN0>(frame);
    }

    void Shoot::control_loop() {
        update_speed();
        decomposition_speed();
        if (no_force) {
            friction[0].give_current = 0;
            friction[1].give_current = 0;
            friction[2].give_current = 0;
            friction[3].give_current = 0;
        } else {
            decomposition_speed();
            for (auto &mot : friction) {
                mot.pid_ctrler.calc(mot.speed, mot.speed_set);
                mot.give_current = (int16_t)mot.pid_ctrler.out;
            }
        }
        send_motor_current();
    }

}  // namespace Shoot