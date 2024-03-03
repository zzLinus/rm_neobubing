#include "chassis.hpp"

namespace Chassis
{
    Chassis::Chassis() {
        can_itrf = std::make_shared<Hardware::Can_interface>();
        // 电机初始化
        motors.assign(4, Hardware::Motor{ Config::M3508_SPEED_PID_CONFIG });
    }

    void Chassis::init() {
        can_itrf->init([&](auto frame) { unpack(frame); }, "can1");
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
        decomposition_speed();
        update_speed();
        if (no_force) {
            for (auto &m : motors) {
                m.give_current = 0;
            }
        } else {
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
        can_frame send_frame{};
        send_frame.can_id = 0x200;
        send_frame.can_dlc = 8;
        for (int i = 0; i < motors.size(); i++) {
            send_frame.data[i * 2] = (motors[i].give_current >> 8);
            send_frame.data[i * 2 + 1] = (motors[i].give_current & 0xff);
        }
        can_itrf->can_send(send_frame);
    }

    void Chassis::decomposition_speed() {
        wheel_speed[0] = vx_set - vy_set + wz_set;
        wheel_speed[1] = -vx_set - vy_set + wz_set;
        wheel_speed[2] = -vx_set + vy_set + wz_set;
        wheel_speed[3] = vx_set + vy_set + wz_set;
    }

    void Chassis::update_speed() {
        for (auto &m : motors) {
            m.speed = Config::CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * (fp32)m.motor_measure.speed_rpm;
            m.accel = Config::CHASSIS_CONTROL_FREQUENCE * m.pid_ctrler.Dbuf;
        }
    }
}  // namespace Chassis
