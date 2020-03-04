#pragma once
#include <cstdint>
#include <gio/gio.h>
#include <iostream>

#define DEFAULT_CHANNELS 3
#define DEFAULT_PIXELFORMAT 8

struct FrameInfo
{
    int Height = {0};
    int Width = {0};
    int Channels = {DEFAULT_CHANNELS};
    int PixelFormat = {DEFAULT_PIXELFORMAT};
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

