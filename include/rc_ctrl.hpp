#ifndef __RC_CTRL__
#define __RC_CTRL__
#include <cstdint>

#include "hardware_callback.hpp"

namespace Io
{

#define RC_Y_CHANNEL          0
#define RC_X_CHANNEL          1
#define RC_YAW_CHANNEL        2
#define RC_PITCH_CHANNEL      3
#define RC_BULLET_BAG_CHANNEL 4
#define RC_SW_UP              ((uint8_t)1)
#define RC_SW_MID             ((uint8_t)3)
#define RC_SW_DOWN            ((uint8_t)2)

    class Rc_ctrl : public Hardware::Callback<Rc_ctrl*>
    {
       public:
        Rc_ctrl();
        void send_control();
		void task();
        ~Rc_ctrl();

       private:
       public:
        int16_t vx;
        int16_t vy;
        int16_t pitch;
        int16_t yaw;
        int16_t bullet_bag;
        uint8_t switch1_state;
        uint8_t switch2_state;

       private:
    };
}  // namespace Io
#endif
