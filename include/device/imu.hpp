#include "device/deviece_base.hpp"
#include "robot.hpp"
#include "types.hpp"
#include "memory"

namespace Device
{
    class IMU : public DeviceBase
    {
       public:
        explicit IMU() = default;

        void init(const std::shared_ptr<Robot::Robot_set> &robot);
        void unpack(const Types::ReceivePacket &pkg);

       private:
        std::shared_ptr<Robot::Robot_set> robot_set;
    };
}