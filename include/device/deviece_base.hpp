#pragma once

#include "chrono"
#include "config.hpp"

namespace Device
{
    class DeviceBase
    {
       public:
        explicit DeviceBase(uint32_t offline_time_t);
        explicit DeviceBase();
        bool offline();

       protected:
        void update_time();

       private:
        using time_point = typename std::chrono::system_clock::time_point;

        uint32_t offline_time;
        time_point last_time;
    };
}  // namespace Device
