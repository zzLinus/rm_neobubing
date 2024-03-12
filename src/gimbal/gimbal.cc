#include "gimbgal.hpp"

namespace Gimbal
{

    Gimbal::Gimbal()
        : yaw_motor(Config::YAW_SPEED_PID_CONFIG),
          pitch_motor(Config::PITCH_SPEED_PID_CONFIG),
          yaw_absolute_pid(Config::GIMBAL_YAW_ABSOLUTE_PID_CONFIG),
          pitch_absolute_pid(Config::GIMBAL_PITCH_ABSOLUTE_PID_CONFIG),
          yaw_relative_pid(Config::GIMBAL_YAW_RELATIVE_PID_CONFIG),
          pitch_relative_pid(Config::GIMBAL_PITCH_RELATIVE_PID_CONFIG) {
    }

    void Gimbal::init(const std::shared_ptr<Robot::Robot_set> &robot) {
        robot_set = robot;

        Robot::hardware->register_callback<CAN0>(0x205, [&](const auto &frame) { yaw_motor.unpack(frame); });
        Robot::hardware->register_callback<CAN0>(0x206, [&](const auto &frame) { pitch_motor.unpack(frame); });
    }

    void Gimbal::start_init_loop() {
        update_data();
        init_yaw_set = robot_set->yaw_relative;
        init_pitch_set = robot_set->ins_pitch;
    }

    void Gimbal::init_loop() {
        update_data();
        init_yaw_set += UserLib::rad_format(0.f - robot_set->yaw_relative) * Config::GIMBAL_INIT_YAW_SPEED;
        init_pitch_set += UserLib::rad_format(0.f - robot_set->ins_pitch) * Config::GIMBAL_INIT_PITCH_SPEED;
        init_yaw_set = std::clamp(init_yaw_set, -0.1f, 0.1f);

        yaw_relative_pid.calc(robot_set->yaw_relative, init_yaw_set);
        yaw_motor.speed_set = yaw_relative_pid.out;
        yaw_motor.pid_ctrler.calc(yaw_gyro, yaw_motor.speed_set);
        yaw_motor.give_current = (int16_t)yaw_motor.pid_ctrler.out;

        // pitch_absolute_pid.calc(robot_set->ins_pitch, init_pitch_set);
        // pitch_motor.speed_set = pitch_absolute_pid.out;
        // pitch_motor.pid_ctrler.calc(pitch_gyro, pitch_motor.speed_set);
        // pitch_motor.give_current = (int16_t)pitch_motor.pid_ctrler.out;

        //        pitch_relative_pid.calc(robot_set->pitch_relative, init_pitch_set);
        //        pitch_motor.speed_set = pitch_relative_pid.out;
        //        pitch_motor.pid_ctrler.calc(pitch_gyro, pitch_motor.speed_set);
        //        pitch_motor.give_current = (int16_t)pitch_motor.pid_ctrler.out;
        // robot_set->yaw_relative << ' ' << robot_set->ins_pitch << std::endl;

        if (fabs(robot_set->yaw_relative) < Config::GIMBAL_INIT_EXP &&
            fabs(robot_set->ins_pitch) < Config::GIMBAL_INIT_EXP) {
            init_stop_times += 1;
        } else {
            init_stop_times = 0;
        }
        inited = init_stop_times >= Config::GIMBAL_INIT_STOP_TIME;
        send_motor_current();
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

            pitch_absolute_pid.calc(robot_set->ins_pitch, pitch_set);
            pitch_motor.speed_set = pitch_absolute_pid.out;
            pitch_motor.pid_ctrler.calc(pitch_gyro, pitch_motor.speed_set);
            pitch_motor.give_current = (int16_t)pitch_motor.pid_ctrler.out;
        }
        send_motor_current();
    }

    void Gimbal::update_data() {
        yaw_motor.speed = Config::RPM_TO_RAD_S * (fp32)yaw_motor.motor_measure.speed_rpm;
        pitch_motor.speed = Config::RPM_TO_RAD_S * (fp32)pitch_motor.motor_measure.speed_rpm;

        robot_set->yaw_relative =
            Config::M6020_ECD_TO_RAD * ((fp32)yaw_motor.motor_measure.ecd - Config::GIMBAL_YAW_OFFSET_ECD);
        robot_set->pitch_relative =
            Config::M6020_ECD_TO_RAD * ((fp32)pitch_motor.motor_measure.ecd - Config::GIMBAL_PITCH_OFFSET_ECD);

        // LOG_INFO("pitch angle %f\n", robot_set->pitch_relative);
        yaw_gyro = std::sin(robot_set->pitch_relative) * robot_set->ins_roll_v -
                   std::cos(robot_set->pitch_relative) * robot_set->ins_yaw_v;
        pitch_gyro = robot_set->ins_pitch_v;
    }

    void Gimbal::send_motor_current() {
        can_frame send_frame{};
        send_frame.can_id = 0x1FF;
        send_frame.can_dlc = 8;
        send_frame.data[0] = (yaw_motor.give_current >> 8);
        send_frame.data[1] = (yaw_motor.give_current & 0xff);
        send_frame.data[2] = (pitch_motor.give_current >> 8);
        send_frame.data[3] = (pitch_motor.give_current & 0xff);
        Robot::hardware->send<CAN0>(send_frame);
    }

}  // namespace Gimbal
