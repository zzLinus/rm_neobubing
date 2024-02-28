#include "pid_controller.hpp"
#include "types.hpp"

namespace Config
{
    const char CAN_CHANNEL[] = "can0";

    // NOTE: PID CONFIG

    const typename Pid::Pid_config M3508_SPEED_PID_CONFIG{
        Pid::PID_MODE::PID_POSITION,  // PID_MODE
        15000.0,                      // KP
        10.0f,                        // KI
        0.0f,                         // KD
        14000.0f,                     // MAX_OUT
        2000.0f,                      // MAX_IOUT
    };

    // chassis follow angle PID
    // 底盘旋转跟随PID
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KP = 20.0f;  // 原始40.0f
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KI = 0.0f;
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_KD = 100.0f;  // 原始0.0f
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT = 6.0f;
    const fp32 CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT = 0.2f;

    // NOTE: chassis
    // chassis no follow angle PID
    // 底盘旋转不跟随PID
    // NOTE: const static fp32 chassis_no_follow_yaw_pid[3] = {4.5, 0.0002, 0}; ZHANG
    //       const static fp32 chassis_no_follow_yaw_pid[3] = {3.5, 0.0002, 0}; HUAN
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KP = 4.5f;  // 原始40.0f
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KI = 0.0002f;
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_KD = 0.0f;  // 原始0.0f
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_OUT = 6.0f;
    const fp32 CHASSIS_NO_FOLLOW_GIMBAL_PID_MAX_IOUT = 0.2f;

    const fp32 CHASSIS_WZ_SPIN = 7;

    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VX = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_VY = 0.25f;
    const fp32 MOTOR_SPEED_TO_CHASSIS_SPEED_WZ = 0.25f;
    const fp32 MOTOR_DISTANCE_TO_CENTER = 0.2f;

    // m3508 rmp change to chassis speed,
    // m3508转化成底盘速度(m/s)的比例，
    const fp32 CHASSIS_MOTOR_RPM_TO_VECTOR_SEN = 0.000415809748903494517209f;
    const fp32 CHASSIS_CONTROL_FREQUENCE = 500.0f;

    // NOTE: chassis task control time 0.002s
    // 底盘任务控制间隔 0.002s
    const fp32 CHASSIS_CONTROL_TIME = 0.002f;
    const fp32 CHASSIS_ACCEL_X_NUM = 0.1666666667f;
    const fp32 CHASSIS_ACCEL_Y_NUM = 0.3333333333f;

    //
    const fp32 RAMP_KEY_ADD_VX = 0.06f;
    const fp32 RAMP_KEY_ADD_VY = 0.06f;
    // 小陀螺缓启停的增量
    // 小陀螺缓启停的增量
    const fp32 RAMP_SPIN_INC = 0.06f;
    const fp32 RAMP_SPIN_DEC = 0.06f;

    const int32_t OPEN_LOOP_MAX_SPEED = 0x0aff;
}  // namespace Config