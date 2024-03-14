#include <cmath>

#include "ramp.hpp"

namespace UserLib
{
    void Ramp::update(fp32 input) {
        if(std::fabs(out - input) <= acc) {
            out = input;
        }
        else {
            out += (input > out ? acc : -acc);
        }
    }

    Ramp::Ramp(fp32 acc, fp32 dt) : acc(acc * dt), dt(dt) { }

    void Ramp::setAcc(fp32 acc_t) {
        this->acc = acc_t * dt;
    }

    void Ramp::clear() {
        out = 0.f;
    }

    void Ramp::clear(fp32 set_v) {
        out = set_v;
    }
}  // namespace Ramp
