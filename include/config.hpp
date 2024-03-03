#pragma once
#include "pid_controller.hpp"
#include "types.hpp"

namespace Config
{
    const char CAN_CHANNEL[] = "can1";

    // NOTE: PID CONFIG

    const typename Pid::Pid_config M3508_SPEED_PID_CONFIG{
        15000.0f,  // KP
        10.0f,     // KI
        0.0f,      // KD
        14000.0f,  // MAX_OUT
        2000.0f,   // MAX_IOUT
    };

    const typename Pid::Pid_config CHASSIS_FOLLOW_GIMBAL_PID_CONFIG{
        20.0f,   // KP
        0.0f,    // KI
        100.0f,  // KD
        6.0f,    // MAX_OUT
        0.2f,    // MAX_IOUT
    };

    const typename Pid::Pid_config GIMBAL_YAW_ABSOLUTE_PID_CONFIG{
        10.0f,  // KP
        3.0f,   // KI
        1.f,    // KD
        10.0f,  // MAX_OUT
        0.0f,   // MAX_IOUT
    };          // MAX_IOUT

//    const typename Pid::Pid_config GIMBAL_YAW_ABSOLUTE_PID_CONFIG{
//        8300.0f,   // KP
//        10.0f,     // KI
//        1.f,       // KD
//        15000.0f,  // MAX_OUT
//        4000.0f,   // MAX_IOUT
//    };

    const typename Pid::Pid_config GIMBAL_PITCH_ABSOLUTE_PID_CONFIG{
        15.0f,  // KP
        0.0f,   // KI
        0.0f,   // KD
        10.0f,  // MAX_OUT
        0.0f,   // MAX_IOUT
    };

    const typename Pid::Pid_config YAW_SPEED_PID_CONFIG{
        40.0f, 3.0f, 5.0f, 30000.0f, 5000.0f,
    };

    constexpr fp32 CHASSIS_SPIN_SPEED = 7;

    // NOTE: chassis
    // chassis no follow angle PID
    // 底盘旋转不跟随PID
    // NOTE: const static fp32 chassis_no_follow_yaw_pid[3] = {4.5, 0.0002, 0}; ZHANG
    //       const static fp32 chassis_no_follow_yaw_pid[3] = {3.5, 0.0002, 0}; HUAN

    const fp32 CHASSIS_WZ_SPIN = 7;

    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VX = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VY = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_WZ = 0.25f;
    const fp32 MOTOR_DISTANCE_TO_CENTER = 0.2f;

    // m3508 rmp change to chassis speed,
    // m3508转化成底盘速度(m/s)的比例，
    constexpr fp32 CHASSIS_MOTOR_RPM_TO_VECTOR_SEN = 0.000415809748903494517209f;
    constexpr fp32 M6020_ECD_TO_RAD = 2.f * M_PIf / 8192.f;
    constexpr fp32 RPM_TO_RAD_S = 2.f * M_PIf / 60.f;
    const fp32 CHASSIS_CONTROL_FREQUENCE = 500.0f;

    // NOTE: chassis task control time 0.002s
    // 底盘任务控制间隔 0.002s
    const fp32 CHASSIS_CONTROL_TIME = 0.002f;
    const fp32 CHASSIS_ACCEL_X_NUM = 0.1666666667f;
    const fp32 CHASSIS_ACCEL_Y_NUM = 0.3333333333f;

}  // namespace Config
