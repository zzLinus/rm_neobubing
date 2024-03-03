#pragma once

#include "types.hpp"

namespace Robot
{
    struct Robot_set
    {
        /** chassis_control **/
        fp32 vx_set = 0.f;
        fp32 vy_set = 0.f;
        fp32 wz_set = 0.f;

        /** gimbal_control **/
        fp32 yaw_set = 0.f;
        fp32 pitch_set = 0.f;

        /** IMU **/
        fp32 ins_yaw = 0.f;
        fp32 ins_pitch = 0.f;
        fp32 ins_roll = 0.f;
        fp32 ins_yaw_v = 0.f;
        fp32 ins_pitch_v = 0.f;
        fp32 ins_roll_v = 0.f;

        /** other **/
        fp32 yaw_relative = 0.f;
        fp32 pitch_relative = 0.f;
        Types::ROBOT_MODE mode = Types::ROBOT_MODE::ROBOT_NO_FORCE;
    };
}

