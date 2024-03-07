#include <iostream>

#include "io.hpp"
#include "robot_controller.hpp"
#include "serial_interface.hpp"
#include "thread"
#include "types.hpp"
#include "utils.hpp"

using namespace std::chrono;

int main(int argc, char **argv) {
    Robot::Robot_ctrl robot;
    robot.start();

    Io::Server_socket_interface socket_intrf(robot.robot_set);
    std::thread socket_thread(&Io::Server_socket_interface::task, &socket_intrf);

    try {
        Hardware::Serial_interface<Types::ReceivePacket> serial;
        std::thread serial_thread(&Hardware::Serial_interface<Types::ReceivePacket>::task, &serial);
        // auto copySerialData = [&]() {
        //     UserLib::RealRad rad;

        //    robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_FOLLOW_GIMBAL;

        //    while (true) {
        //        auto now = system_clock::now();
        //        auto step = duration_cast<milliseconds>(now - start).count();
        //        robot.robot_set->ins_yaw = serial.rp.yaw;
        //        robot.robot_set->ins_pitch = serial.rp.pitch;
        //        robot.robot_set->ins_roll = serial.rp.roll;
        //        robot.robot_set->ins_yaw_v = serial.rp.yaw_v;
        //        robot.robot_set->ins_pitch_v = serial.rp.pitch_v;
        //        robot.robot_set->ins_roll_v = serial.rp.roll_v;
        //        robot.robot_set->mode = Types::ROBOT_MODE::ROBOT_NOT_FOLLOW;
        //        rad.update(
        //            UserLib::rad_format((fp32)robot.gimbal.yaw_motor.motor_measure.ecd * Config::M6020_ECD_TO_RAD));
        //        // LOG_INFO("%f,%f\n", robot.robot_set->yaw_relative, robot.robot_set->pitch_relative);

        //        std::this_thread::sleep_for(2ms);
        //    }
        //};
        // std::thread copy_thread(copySerialData);

        // copy_thread.join();
        serial_thread.join();
    } catch (serial::IOException) {
        LOG_ERR("there's no such serial device\n");
    }

    socket_thread.join();
    robot.join();
    // robot.start_init();
    // robot.init_join();

    return 0;
}
