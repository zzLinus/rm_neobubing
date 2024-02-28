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

    bool Can_interface::can_dump(Types::debug_info_t *debug) {
        for (;;) {
            if (init_flag != false) {
//                debug->err = false;
                // read CAN frame
                if (read(soket_id, &frame_r, sizeof(can_frame)) <= 0) {
                    perror("Error reading CAN frame");
                    return Status::ERROR;
                } else {
                    // printf("not reading!\n");
                }

                callback_fun(frame_r);

//                debug->can_f.can_id = frame_r.can_id;
//                debug->can_f.can_dlc = frame_r.can_dlc;

//                for (int i = 0; i < frame_r.can_dlc; i++) {
//                    debug->can_f.data[i] = frame_r.data[i];
//                }
            }
//            debug->err = true;
        }
    }

    bool Can_interface::can_send(uint64_t can_pkg) {
        // eg : test can pkd <01BB117001BBEE90>
        frame_w.can_id = 0x200;
        frame_w.can_dlc = 8;
        frame_w.data[1] = (uint8_t)(can_pkg >> 0);
        frame_w.data[0] = (uint8_t)(can_pkg >> 8);
        frame_w.data[3] = (uint8_t)(can_pkg >> 16);
        frame_w.data[2] = (uint8_t)(can_pkg >> 24);
        frame_w.data[5] = (uint8_t)(can_pkg >> 32);
        frame_w.data[4] = (uint8_t)(can_pkg >> 40);
        frame_w.data[7] = (uint8_t)(can_pkg >> 48);
        frame_w.data[6] = (uint8_t)(can_pkg >> 56);

        /* send CAN frame */
        write(soket_id, &frame_w, sizeof(can_frame));

        return true;
    }

}  // namespace Hardware
