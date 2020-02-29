#include "icamera.h"

ICamera::ICamera()
{

}

ICamera::~ICamera()
{
}

void ICamera::Stop()
{
    stopFlag.store(true);
}

CurrentFrameInfo ICamera::currentFrameInfo() const
{
    while(!protector);
    return currentFrameInfo_;
}

bool ICamera::getStopFlag()
{
    return stopFlag.load();
}

void ICamera::unsetStopFlag()
{
    stopFlag.store(false);
    condition.notify_one();
}

bool ICamera::getIsPropsReady() const
{
    return isPropsReady;
}




CameraInfo ICamera::getCameraInfo() const
{
    return cameraInfo_;
}

void ICamera::setCurrentFrameInfo(const CurrentFrameInfo &currentFrameInfo)
{
    Stop();
    currentFrameInfo_ = currentFrameInfo;
    Start();
}

void ICamera::setCameraInfo(const CameraInfo &cameraInfo)
{
    cameraInfo_ = cameraInfo;
}


