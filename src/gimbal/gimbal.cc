#include "gimbgal.hpp"
#include "iostream"

namespace Gimbal
{

    Gimbal::Gimbal()
        : yaw_motor(Config::YAW_SPEED_PID_CONFIG),
          pitch_motor(Config::GIMBAL_PITCH_ABSOLUTE_PID_CONFIG),
          yaw_absolute_pid(Config::GIMBAL_YAW_ABSOLUTE_PID_CONFIG),
          pitch_absolute_pid(Config::GIMBAL_PITCH_ABSOLUTE_PID_CONFIG) {
        can_itrf = std::make_shared<Hardware::Can_interface>();
    }

    void Gimbal::init(const std::shared_ptr<Robot::Robot_set>& robot) {
        can_itrf->init([&](auto frame) { unpack(frame); }, "can0");
        robot_set = robot;
    }

    void Gimbal::control_loop() {
        update_data();
        if (no_force) {
            yaw_motor.give_current = 0.f;
            pitch_motor.give_current = 0.f;
        } else {
            yaw_absolute_pid.calc(robot_set->ins_yaw, yaw_set);
            yaw_motor.speed_set = yaw_absolute_pid.out;
            yaw_motor.pid_ctrler.calc(yaw_gyro, yaw_motor.speed_set);
            yaw_motor.give_current = (int16_t)yaw_motor.pid_ctrler.out;

            //            pitch_absolute_pid.calc(robot_set->ins_pitch, pitch_set);
            //            pitch_motor.speed_set = pitch_absolute_pid.out;
            //            pitch_motor.pid_ctrler.calc(pitch_gyro, pitch_motor.speed_set);
            //            pitch_motor.give_current = (int16_t)pitch_motor.pid_ctrler.out;
        }
        send_motor_current();
    }

    void Gimbal::unpack(const can_frame& frame) {
        if (frame.can_id != 0x205 && frame.can_id != 0x206) {
            return;
        }
        auto& motor_t = ((frame.can_id == 0x205) ? yaw_motor : pitch_motor).motor_measure;
        motor_t.last_ecd = motor_t.last_ecd;
        motor_t.ecd = (uint16_t)(frame.data[0] << 8 | frame.data[1]);
        motor_t.speed_rpm = (uint16_t)(frame.data[2] << 8 | frame.data[3]);
        motor_t.given_current = (uint16_t)(frame.data[4] << 8 | frame.data[5]);
        motor_t.temperate = frame.data[6];
    }

    void Gimbal::update_data() {
        yaw_motor.speed = Config::RPM_TO_RAD_S * (fp32)yaw_motor.motor_measure.speed_rpm;
        pitch_motor.speed = Config::RPM_TO_RAD_S * (fp32)pitch_motor.motor_measure.speed_rpm;

        robot_set->yaw_relative = UserLib::rad_format(
            Config::M6020_ECD_TO_RAD * ((fp32)yaw_motor.motor_measure.ecd - Config::GIMBAL_YAW_OFFSET_ECD));
        robot_set->pitch_relative = UserLib::rad_format(
            Config::M6020_ECD_TO_RAD * ((fp32)pitch_motor.motor_measure.ecd - Config::GIMBAL_PITCH_OFFSET_ECD));

        yaw_gyro = std::cos(robot_set->pitch_relative) * robot_set->ins_roll_v
                   - std::sin(robot_set->pitch_relative) * robot_set->ins_yaw_v;
        pitch_gyro = robot_set->ins_pitch_v;
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
