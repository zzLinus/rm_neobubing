#include "gimbgal.hpp"
#include "iostream"

namespace Gimbal
{

    Gimbal::Gimbal()
        : yaw_motor(Config::GIMBAL_YAW_ABSOLUTE_PID_CONFIG),
          pitch_motor(Config::GIMBAL_PITCH_ABSOLUTE_PID_CONFIG) {
    }

    void Gimbal::init(const std::shared_ptr<Robot::Robot_set>& robot) {
        robot_set = robot;
    }

    void Gimbal::control_loop() {

    }

}  // namespace Gimbal
