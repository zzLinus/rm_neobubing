#include <cmath>

#include "types.hpp"

namespace UserLib
{
    fp32 rad_format(fp32 ang);

    class RealRad {
       public:
        void update(fp32 ref);
       public:
        int count = 0;
        fp32 now = 0.0;
        fp32 last = 0.0;
    };
}