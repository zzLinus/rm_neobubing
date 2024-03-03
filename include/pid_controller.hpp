#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__

#include <climits>
#include <cmath>
#include <algorithm>

#include "types.hpp"
#include "user_lib.hpp"

namespace Pid
{

    enum PID_MODE
    {
        PID_POSITION,
        PID_DELTA,
    };

    struct Pid_config
    {
        Pid_config(fp32 kp, fp32 ki, fp32 kd, fp32 max_out, fp32 max_iout);

        // PID 三参数
        fp32 kp;
        fp32 ki;
        fp32 kd;

        fp32 max_out;   // 最大输出
        fp32 max_iout;  // 最大积分输出
    };

    class Pid_position : public Pid_config
    {
       public:
        ~Pid_position() = default;
        explicit Pid_position(const Pid_config &config);

        void calc(fp32 ref, fp32 set);
        void clean();

       public:
        fp32 out = 0.f;
        fp32 Pout = 0.f;
        fp32 Iout = 0.f;
        fp32 Dout = 0.f;
        fp32 Dbuf = 0.f;   // 微分项 0最新 1上一次 2上上次
        fp32 error[2] = {};  // 误差项 0最新 1上一次 2上上次
    };

    class Pid_rad : public Pid_config
    {
       public:
        explicit Pid_rad(const Pid_config &config);
        ~Pid_rad() = default;
//        void calc(fp32 get, fp32 set, fp32 error_delta);
        void calc(fp32 get, fp32 set);

       public:
        fp32 out = 0.f;
        fp32 Pout = 0.f;
        fp32 Iout = 0.f;
        fp32 Dout = 0.f;
        fp32 err = 0.f;
        fp32 last_err = 0.f;

    };
}  // namespace Pid

#endif
