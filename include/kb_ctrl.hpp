#ifndef __KB_CTRL__
#define __KB_CTRL__

#include <chrono>
#include <cmath>

#include "ramp_controller.hpp"
#include "config.hpp"

namespace Input
{
    class Kb_ctrl
    {
       public:
        Kb_ctrl();
        ~Kb_ctrl();
        fp32 event_handler(Types::Kb_event event, fp32 spdslider);

       private:
        void ramp_calc(Types::ramp_t *ramp_source_type, fp32 input);

       public:
        Types::RC_ctrl_t *rc_ctrl;
        Ramp::Ramp_ctrller *kb_vx_ramp;  // 用于键盘控制的斜波函数
        Ramp::Ramp_ctrller *kb_vy_ramp;  // 用于键盘控制的斜波函数

       private:
    };
}  // namespace Input

#endif
