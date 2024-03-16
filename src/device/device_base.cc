#include "device/deviece_base.hpp"

using namespace std::chrono;

namespace Device
{
    DeviceBase::DeviceBase() :
          last_time(system_clock::now()), offline_time(Config::DEFAULT_OFFLINE_TIME) {
    }

    DeviceBase::DeviceBase(uint32_t offline_time_t) :
          last_time(system_clock::now()), offline_time(offline_time_t) {
    }

    bool DeviceBase::offline() {
        return duration_cast<milliseconds>(system_clock::now() - last_time).count() >= offline_time;
    }

    void DeviceBase::update_time() {
        last_time = system_clock::now();
    }
}  // namespace Device