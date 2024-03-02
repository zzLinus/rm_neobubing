#include "can.hpp"

#include <cstring>

#include "iostream"

namespace Hardware
{
    Can_interface::Can_interface() {
        addr = new sockaddr_can;
        ifr = new ifreq;
        soket_id = -1;
        init_flag = false;
    }

    void Can_interface::init(const CallbackType &callback) {
        callback_fun = callback;
        // create CAN socket
        if ((soket_id = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
            perror("Error while creating socket");
            exit(-1);
        }

        struct can_filter rfilter[4];
        rfilter[0].can_id = 0x201;
        rfilter[0].can_mask = 0x3ff;
        rfilter[1].can_id = 0x202;
        rfilter[1].can_mask = 0x3ff;
        rfilter[2].can_id = 0x203;
        rfilter[2].can_mask = 0x3ff;
        rfilter[3].can_id = 0x204;
        rfilter[3].can_mask = 0x3ff;
        rfilter[3].can_id = 0x205;
        rfilter[3].can_mask = 0x3ff;
        setsockopt(soket_id, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));

        std::strcpy(ifr->ifr_name, Config::CAN_CHANNEL);
        ioctl(soket_id, SIOCGIFINDEX, ifr);

        addr->can_family = AF_CAN;
        addr->can_ifindex = ifr->ifr_ifindex;

        // bind CAN socket to the given interface
        if (bind(soket_id, (sockaddr *)addr, sizeof(*addr)) < 0) {
            perror("Error in socket bind");
            exit(-1);
        }
        init_flag = true;
    }

    Can_interface::~Can_interface() {
        delete addr;
        delete ifr;
    }

    bool Can_interface::can_dump() {
        for (;;) {
            if (init_flag) {
                // read CAN frame
                if (read(soket_id, &frame_r, sizeof(can_frame)) <= 0) {
                    perror("Error reading CAN frame");
                    return Status::ERROR;
                } else {
                    // printf("not reading!\n");
                }
                callback_fun(frame_r);
            }
        }
    }

    bool Can_interface::can_send(const can_frame &frame) {
        /* send CAN frame */
        write(soket_id, &frame, sizeof(can_frame));

        return true;
    }

}  // namespace Hardware
