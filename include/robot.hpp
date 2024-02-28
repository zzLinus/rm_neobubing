#pragma once

#include "types.hpp"

namespace Robot
{
    struct Robot_set
    {
        fp32 vx_set = 0.f;
        fp32 vy_set = 0.f;
        fp32 v_yaw_set = 0.f;
        fp32 v_pitch_set = 0.f;
        fp32 chassis_relative_angle = 0.f;
        Types::ROBOT_MODE mode = Types::ROBOT_MODE::ROBOT_NO_FORCE;
    };
}

