#include "motor.hpp"

namespace Hardware
{

    //    template<>
    //    Motor<Pid::Pid_position>::Motor(const Pid::Pid_config &config) : pid_ctrler(config) {
    //    }
    //
    //    template<>
    //    Motor<Pid::Pid_rad>::Motor(const Pid::Pid_config &config) : pid_ctrler(config) {
    //    }

    void Motor::unpack(const can_frame& frame) {
        update_time();
        auto& motor_t = motor_measure;
        motor_t.last_ecd = motor_t.last_ecd;
        motor_t.ecd = (uint16_t)(frame.data[0] << 8 | frame.data[1]);
        motor_t.speed_rpm = (uint16_t)(frame.data[2] << 8 | frame.data[3]);
        motor_t.given_current = (uint16_t)(frame.data[4] << 8 | frame.data[5]);
        motor_t.temperate = frame.data[6];
    }
}  // namespace Hardware
