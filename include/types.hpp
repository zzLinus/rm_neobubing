#ifndef __TYPES__
#define __TYPES__
#include <linux/can.h>

#include <cstdint>

#include "string"

// NOTE: defines and type defines
typedef signed char int8_t;
typedef signed short int int16_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

namespace Status
{
    const bool OK = true;
    const bool ERROR = false;
}  // namespace Status

typedef enum
{
    CAN0 = 0,
    CAN1,
    SER1,
    SOCKET,
	RC_CTRL
} HardwareId;

namespace Types
{
    typedef struct ReceivePacket
    {
        float yaw;
        float pitch;
        float roll;
        float yaw_v;
        float pitch_v;
        float roll_v;
        int16_t ch[5];
        char s[2];
    } __attribute__((packed)) ReceivePacket;

    typedef struct
    {
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num[1];        // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } first_order_filter_type_t;

    enum Kb_event
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        SPIN_R,
        SPIN_L,
        STOP_X,
        STOP_Y,
    };

    typedef struct
    {
        fp32 vx;
        fp32 vy;
        fp32 wz;
        fp32 wheel_speed[4];
        can_frame can_f;
        uint64_t pkg;
        bool err;
        std::string debuginfo1;
        std::string debuginfo2;
        std::string debuginfo3;

    } debug_info_t;

    typedef struct
    {
        struct
        {
            int16_t ch[5];
            char s[2];
        } rc;
        struct
        {
            int16_t x;
            int16_t y;
            int16_t z;
            uint8_t press_l;
            uint8_t press_r;
        } mouse;
        struct
        {
            bool q;  //  spin clock wise
            bool f;  //  spin counter clock wise
            fp32 speed_x;
            fp32 speed_y;
            int16_t v;
        } key;

    } RC_ctrl_t;
    enum ROBOT_MODE
    {
        ROBOT_NO_FORCE,
        ROBOT_FOLLOW_GIMBAL,
        ROBOT_NOT_FOLLOW
    };

    typedef enum
    {
        CHSMODE_ZERO_FORCE = 0x1 << 0,  // chassis will be like no power,底盘无力, 跟没上电那样
        CHSMODE_NO_MOVE = 0x1 << 1,     // chassis will be stop,底盘保持不动
        CHSMODE_FOLLOW_GIMBAL_YAW = 0x1 << 2,
        CHSMODE_FOLLOW_CHASSIS_YAW = 0x1 << 3,
        // speed 底盘不跟随角度，角度是开环的，但轮子是有速度环
        // 底盘不跟随云台，但左右推左摇杆时底盘也会向左向右转
        CHSMODE_NO_FOLLOW_GIMBAL_YAW = 0x1 << 4,
        CHSMODE_OPEN = 0x1 << 5,  //  遥控器的值乘以比例成电流值 直接发送到can总线上
        CHSMODE_SPIN = 0x1 << 6,  // 新增：小陀螺模式
    } mode_e;

    typedef struct
    {
        fp32 input;         // 输入数据
        fp32 out;           // 滤波输出的数据
        fp32 num;           // 滤波参数
        fp32 frame_period;  // 滤波的时间间隔 单位 s
    } fof_t;

    typedef struct
    {
    } ramp_t;
}  // namespace Types

#endif
