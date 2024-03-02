#include "gimbgal.hpp"
#include "iostream"

namespace Gimbal
{

    Gimbal::Gimbal()
        : yaw_motor(Config::YAW_SPEED_PID_CONFIG),
          pitch_motor(Config::GIMBAL_PITCH_ABSOLUTE_PID_CONFIG),
          yaw_absolute_pid(Config::GIMBAL_YAW_ABSOLUTE_PID_CONFIG) {
        can_itrf = std::make_shared<Hardware::Can_interface>();
    }

    void Gimbal::init() {
        can_itrf->init([&](auto frame) { unpack(frame); });
    }

    void Gimbal::control_loop() {
        update_speed();
        if(no_force) {
            yaw_motor.give_current = 0.f;
            pitch_motor.give_current = 0.f;
        }
        else {
            yaw_motor.speed_set = v_yaw_set;
            yaw_motor.pid_ctrler.calc(yaw_motor.speed, yaw_motor.speed_set);
            yaw_motor.give_current = (int16_t)yaw_motor.pid_ctrler.out;
        }
        send_motor_current();
    }

    void Gimbal::unpack(const can_frame& frame) {
        auto &motor_t = ((frame.can_id == 0x205) ? yaw_motor : pitch_motor).motor_measure;
        motor_t.last_ecd = motor_t.last_ecd;
        motor_t.ecd = (uint16_t)(frame.data[0] << 8 | frame.data[1]);
        motor_t.speed_rpm = (uint16_t)(frame.data[2] << 8 | frame.data[3]);
        motor_t.given_current = (uint16_t)(frame.data[4] << 8 | frame.data[5]);
        motor_t.temperate = frame.data[6];
    }

    void Gimbal::update_speed() {
        yaw_motor.speed = Config::RPM_TO_RAD_S * (fp32)yaw_motor.motor_measure.speed_rpm;
        pitch_motor.speed = Config::RPM_TO_RAD_S * (fp32)pitch_motor.motor_measure.speed_rpm;
    }
    void Gimbal::send_motor_current() {
        can_frame send_frame{};
        send_frame.can_id = 0x1FF;
        send_frame.can_dlc = 8;
        send_frame.data[0] = (yaw_motor.give_current >> 8);
        send_frame.data[1] = (yaw_motor.give_current & 0xff);
        can_itrf->can_send(send_frame);
    }

}  // namespace Gimbal
