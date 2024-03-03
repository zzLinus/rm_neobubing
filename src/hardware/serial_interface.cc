#include "serial_interface.hpp"

#include <iostream>

namespace Hardware
{
    Serial_interface::Serial_interface()
        : serial::Serial("/dev/ttyACM0", 115200, serial::Timeout::simpleTimeout(1000)) {
    }

    Serial_interface::~Serial_interface() {
    }

    inline void Serial_interface::enumerate_ports() {
        std::vector<serial::PortInfo> devices_found = serial::list_ports();
        auto iter = devices_found.begin();

        while (iter != devices_found.end()) {
            serial::PortInfo device = *iter++;
            printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str());
        }
    }

    inline void Serial_interface::fromVector(const uint8_t *data, ReceivePacket *pkg) {
        for (size_t i = 0; i < sizeof(ReceivePacket); ++i) {
            ((uint8_t *)pkg)[i] = data[i];
        }
    }

    inline int Serial_interface::unpack() {
        memcpy(buffer, read(sizeof(ReceivePacket)).c_str(), sizeof(ReceivePacket));
        fromVector(buffer, &rp);
//        printf("serial info: %f %f %f\n", rp.yaw, rp.pitch, rp.roll);

        return 0;
    }

    void Serial_interface::task() {
        while (true) {
            if (isOpen()) {
                read(&header, 1);
                if (header == 0xAB)
                    unpack();
            } else {
                enumerate_ports();
                return;
            }
        }
    }
}  // namespace Hardware
