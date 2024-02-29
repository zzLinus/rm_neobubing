#include "user_lib.hpp"

namespace UserLib
{
    fp32 rad_format(fp32 ang) {
        fp32 ans = fmodf(ang + M_PIf, M_PIf * 2.f);
        return (ans < 0.f) ? ans + M_PIf : ans - M_PIf;
    }
}