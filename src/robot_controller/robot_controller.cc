#include "robot_controller.hpp"
#include "can.hpp"

namespace Robot
{
    Robot_ctrl::Robot_ctrl() {
        robot_set = std::make_shared<Robot_set>();
        chassis_ctrl.init(robot_set);
    }

    std::shared_ptr<const Robot_set> Robot_ctrl::get_robot_read() const {
        return robot_set;
    }

    std::shared_ptr<Robot_set> Robot_ctrl::get_robot_write() {
        return robot_set;
    }

    bool Robot_ctrl::start() {
        chassis_can_tread = std::make_unique<std::thread>(
            &Hardware::Can_interface::can_dump,
            chassis_ctrl.chassis.can_itrf,
            (Types::debug_info_t *)nullptr
            );
        chassis_tread = std::make_unique<std::thread>(
            &Chassis::Chassis_ctrl::task,
            &chassis_ctrl
            );
        return true;
    }

};