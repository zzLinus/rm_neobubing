#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__

#include <climits>
#include <cmath>

#include "types.hpp"

namespace Pid
{

    enum PID_MODE
    {
        PID_POSITION,
        PID_DELTA,
    };

    struct Pid_config
    {
        Pid_config(uint8_t mode, fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);
        uint8_t mode;
        // PID 三参数
        fp32 kp;
        fp32 ki;
        fp32 kd;

        fp32 max_out;   // 最大输出
        fp32 max_iout;  // 最大积分输出
    };

    class Pid_ctrl : public Pid_config
    {
       public:
        explicit Pid_ctrl(const Pid_config &config);
        ~Pid_ctrl() = default;

        void calc(fp32 ref, fp32 set);

       private:
       public:
        fp32 out = 0.f;
        fp32 Pout = 0.f;
        fp32 Iout = 0.f;
        fp32 Dout = 0.f;
        fp32 Dbuf[3] = {};   // 微分项 0最新 1上一次 2上上次
        fp32 error[3] = {};  // 误差项 0最新 1上一次 2上上次
       private:
    };

}  // namespace Pid

#endif
