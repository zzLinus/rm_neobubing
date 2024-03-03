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
        12.0f,  // KP
        0.0f,   // KI
        0.3f,    // KD
        10.0f,  // MAX_OUT
        0.0f,   // MAX_IOUT
    };          // MAX_IOUT

    const typename Pid::Pid_config GIMBAL_PITCH_ABSOLUTE_PID_CONFIG{
        15.0f,  // KP
        0.0f,   // KI
        0.0f,   // KD
        10.0f,  // MAX_OUT
        0.0f,   // MAX_IOUT
    };

    const typename Pid::Pid_config YAW_SPEED_PID_CONFIG{
        8200.0f / 3.f,
        15.0f / 4.f,
        0.0f,
        30000.0f / 3.f,
        5000.0f / 3.f,
    };

    // m3508 rmp change to chassis speed,
    // m3508转化成底盘速度(m/s)的比例，
    constexpr fp32 CHASSIS_MOTOR_RPM_TO_VECTOR_SEN = 0.000415809748903494517209f;
    constexpr fp32 M6020_ECD_TO_RAD = 2.f * M_PIf / 8192.f;
    constexpr fp32 RPM_TO_RAD_S = 2.f * M_PIf / 60.f;
    constexpr fp32 CHASSIS_CONTROL_FREQUENCE = 500.0f;

    constexpr fp32 GIMBAL_YAW_OFFSET_ECD = 4200;
    constexpr fp32 GIMBAL_PITCH_OFFSET_ECD = 7875;

}  // namespace Config
