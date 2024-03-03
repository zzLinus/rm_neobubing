#ifndef __SERIAL_INTERFACE__
#define __SERIAL_INTERFACE__

#include "serial/serial.h"

namespace Hardware
{

    template<class T>
    class Serial_interface : serial::Serial
    {
       public:
        Serial_interface();
        ~Serial_interface();
        void task();

       private:
        inline void enumerate_ports();
        inline int unpack();
        inline void fromVector(uint8_t *data, T *pkg);

       public:
        T rp;

       private:
        uint8_t buffer[256];
        uint8_t header;
    };

    template<class T>
    Serial_interface<T>::Serial_interface()
        : serial::Serial(std::string("/dev/ttyACM1"), 115200, serial::Timeout::simpleTimeout(1000)) {
    }

    template<class T>
    Serial_interface<T>::~Serial_interface() {
    }

    template<class T>
    inline void Serial_interface<T>::enumerate_ports() {
        std::vector<serial::PortInfo> devices_found = serial::list_ports();
        std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

        while (iter != devices_found.end()) {
            serial::PortInfo device = *iter++;
            printf("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str());
        }
    }

    template<class T>
    inline void Serial_interface<T>::fromVector(uint8_t *data, T *pkg) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            ((uint8_t *)pkg)[i] = data[i];
        }
    }

    template<class T>
    inline int Serial_interface<T>::unpack() {
        memcpy(buffer, read(sizeof(T)).c_str(), sizeof(T));
        fromVector(buffer, &rp);
        printf("serial info: %f %f %f %f %f %f\n", rp.yaw, rp.pitch, rp.roll, rp.yaw_v, rp.pitch_v, rp.roll_v);

        return 0;
    }

    template<class T>
    void Serial_interface<T>::task() {
        while (1) {
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
#endif
