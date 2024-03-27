#include "motor.hpp"

namespace Hardware
{

    void Motor::unpack(const can_frame& frame) {
        update_time();
        auto& motor_t = motor_measure;
        motor_t.last_ecd = motor_t.last_ecd;
        motor_t.ecd = (uint16_t)(frame.data[0] << 8 | frame.data[1]);
        motor_t.speed_rpm = (uint16_t)(frame.data[2] << 8 | frame.data[3]);
        motor_t.given_current = (uint16_t)(frame.data[4] << 8 | frame.data[5]);
        motor_t.temperate = frame.data[6];
    }

    can_frame get_frame(canid_t can_id, const std::vector<Motor> &mot_list) {
        can_frame frame{};
        frame.can_id = can_id;
        frame.can_dlc = 8;
        for(int i = 0; i < mot_list.size() && i < 4; i++) {
            frame.data[i << 1] = (mot_list[i].give_current >> 8);
            frame.data[i << 1 | 1] = (mot_list[i].give_current & 0xFF);
        }
        return frame;
    }
}  // namespace Hardware
