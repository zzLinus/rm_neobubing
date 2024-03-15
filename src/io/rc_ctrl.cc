#include "rc_ctrl.hpp"

namespace Io
{

    Rc_ctrl::Rc_ctrl() {
    }

    void Rc_ctrl::send_control() {
        callback(this);
    }

    void Rc_ctrl::task() {
    }

    Rc_ctrl::~Rc_ctrl() {
    }

}  // namespace Io
