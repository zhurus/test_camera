#ifndef ICAMERA_H
#define ICAMERA_H

#include <iostream>
#include <future>
//#include "shmm_v1.h"
#include "datastructs.h"





class ICamera
{
public:
    ICamera();
    virtual ~ICamera();
    virtual void Start()=0;
    void Stop();

    CurrentFrameInfo currentFrameInfo() const;
    CameraInfo getCameraInfo() const;
    void setCurrentFrameInfo(const CurrentFrameInfo &currentFrameInfo);
    void setCameraInfo(const CameraInfo &cameraInfo);
    bool getIsPropsReady() const;

protected:
    bool getStopFlag();
    void unsetStopFlag();

    std::condition_variable condition;
    std::mutex mtx;
    std::atomic_bool stopFlag = {false};
    std::atomic_bool isStarted = {false};
    std::atomic_bool protector = {false};
    bool isPropsReady = {false};
    CurrentFrameInfo currentFrameInfo_;
    CameraInfo cameraInfo_;



};

#endif // ICAMERA_H
