#include "chassis.hpp"

namespace Chassis
{
    Chassis::Chassis() {
        can_itrf = std::make_shared<Hardware::Can_interface>();
        // 电机初始化
        motors.assign(4, { Config::M3508_SPEED_PID_CONFIG });
    }

    void Chassis::init(Types::debug_info_t *debug) {
        debugInfo = debug;
        can_itrf->init([&](auto frame) { unpack(frame); });
    }

    void Chassis::unpack(const can_frame &frame) {
        auto &motor_t = motors[frame.can_id - 0x201].motor_measure;
        motor_t.last_ecd = motor_t.last_ecd;
        motor_t.ecd = (uint16_t)(frame.data[0] << 8 | frame.data[1]);
        motor_t.speed_rpm = (uint16_t)(frame.data[2] << 8 | frame.data[3]);
        motor_t.given_current = (uint16_t)(frame.data[4] << 8 | frame.data[5]);
        motor_t.temperate = frame.data[6];
    }

    void Chassis::control_loop() {
        if (no_force) {
            for (auto &m : motors) {
                m.give_current = 0;
            }
        } else {
            decomposition_speed();
            fp32 max_speed = 0.f;
            for (int i = 0; i < 4; i++) {
                motors[i].speed_set = wheel_speed[i];
                max_speed = std::max(max_speed, fabsf(wheel_speed[i]));
            }
            if (max_speed > max_wheel_speed) {
                fp32 speed_rate = max_wheel_speed / max_speed;
                for (auto &m : motors) {
                    m.speed_set *= speed_rate;
                }
            }
            for (auto &m : motors) {
                m.pid_ctrler.calc(m.speed, m.speed_set);
                m.give_current = (int16_t)(m.pid_ctrler.out);
            }
        }
        send_motor_current();
    }

    void Chassis::send_motor_current() {
        uint64_t pkg = 0;
        for (int i = 0; i < motors.size(); i++) {
            pkg = pkg | (((uint64_t)motors[i].give_current & 0xffff) << (16 * i));
        }
        debugInfo->pkg = pkg;
        can_itrf->can_send(pkg);
    }

    void Chassis::decomposition_speed() {
        wheel_speed[0] = vx_set - vy_set + wz_set;
        wheel_speed[1] = -vx_set - vy_set + wz_set;
        wheel_speed[2] = -vx_set + vy_set + wz_set;
        wheel_speed[3] = vx_set + vy_set + wz_set;
    }
}  // namespace Chassis