#include "motor.hpp"

namespace Hardware
{

    Motor::Motor(const Pid::Pid_config &config) : pid_ctrler(config) {
    }

}  // namespace Hardware
