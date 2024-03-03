#ifndef __SERIAL_INTERFACE__
#define __SERIAL_INTERFACE__

#include "serial/serial.h"

namespace Hardware
{

    typedef struct ReceivePacket
    {
        float roll;
        float pitch;
        float yaw;
    } __attribute__((packed)) ReceivePacket;

    class Serial_interface : serial::Serial
    {
       public:
        Serial_interface();
        ~Serial_interface();
        void task();

       private:
        inline void enumerate_ports();
        inline int unpack();
        inline void fromVector(const uint8_t *data, ReceivePacket *pkg);

       public:
        ReceivePacket rp;

       private:
        uint8_t buffer[256];
        uint8_t header;
    };
}  // namespace Hardware

#endif
