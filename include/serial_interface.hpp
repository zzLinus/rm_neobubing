#ifndef __SERIAL_INTERFACE__
#define __SERIAL_INTERFACE__

#include "serial/serial.h"
#include "utils.hpp"

namespace Hardware
{

    template<class T>
    class Serial_interface : serial::Serial
    {
       public:
        using callbackType = std::function<void(int, const T &t)>;

        Serial_interface() = default;
        ~Serial_interface();
        void task();
        void set_callback(const callbackType &fun);

       private:
        inline void enumerate_ports();
        inline int unpack();
        inline void fromVector(const uint8_t *data, T *pkg);

       public:
        T rp;

       private:
        callbackType callback_fun;
        uint8_t buffer[256];
        uint8_t header;
    };
    template<class T>
    void Serial_interface<T>::set_callback(const Serial_interface::callbackType &fun) {
        callback_fun = fun;
    }

//    template<class T>
//    Serial_interface<T>::Serial_interface()
//        : serial::Serial(std::string("/dev/ttyACM0"), 115200, serial::Timeout::simpleTimeout(1000)) {
//    }

    template<class T>
    Serial_interface<T>::~Serial_interface() = default;

    template<class T>
    inline void Serial_interface<T>::enumerate_ports() {
        std::vector<serial::PortInfo> devices_found = serial::list_ports();
        auto iter = devices_found.begin();

        while (iter != devices_found.end()) {
            serial::PortInfo device = *iter++;
            LOG_INFO("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(), device.hardware_id.c_str());
        }
    }

    template<class T>
    inline void Serial_interface<T>::fromVector(const uint8_t *data, T *pkg) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            ((uint8_t *)pkg)[i] = data[i];
        }
    }

    template<class T>
    inline int Serial_interface<T>::unpack() {
        memcpy(buffer, read(sizeof(T)).c_str(), sizeof(T));
        fromVector(buffer, &rp);
        callback_fun(0, rp);

        return 0;
    }

    template<class T>
    void Serial_interface<T>::task() {
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
#endif
