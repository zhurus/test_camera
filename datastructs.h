#pragma once
#include <cstdint>
#include <gio/gio.h>
#include <iostream>
struct FrameInfo
{
    int Height;
    int Width;
    int Channels;
    int PixelFormat;
};
struct CurrentFrameInfo
{
    FrameInfo frameInfo;
    u_int64_t frameNum;
    u_int64_t timeStamp;

};

struct CameraInfo
{
    std::string name;
    std::string vendor;
    std::string type;
    int shmid;
};

