#include "rtspcamera.h"

#include <opencv2/videoio/videoio.hpp>
#include <opencv2/opencv.hpp>
#include "opencv/highgui.h"

RTSPCamera::RTSPCamera(const std::string& url,const std::string& recvFilename):
    m_url(url),m_recvFilename(recvFilename)
{
}

RTSPCamera::~RTSPCamera()
{
}

void RTSPCamera::Start()
{
    using namespace cv;

    isStarted.store(true);
    unsetStopFlag();

    VideoCapture cap;
    if(!cap.open(m_url))
        throw std::runtime_error("RTSPCamera::Start: Failed to connect input video");

    int frame_width = currentFrameInfo_.frameInfo.Width;
    if(!frame_width)
        frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = currentFrameInfo_.frameInfo.Width;
    if(!frame_height)
        frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter writer(
                    m_recvFilename,
                    writer.fourcc('H','2','6','4'), // FFMPEG Codec
                    fps,
                    Size2d(frame_width,frame_height)
                );

    while(true) {
        std::unique_lock<std::mutex> lock(mtx);
        if(stopFlag)
            condition.wait(lock);

        Mat frame(Size(frame_width,frame_height),
                  CV_MAKETYPE(currentFrameInfo_.frameInfo.PixelFormat,
                              currentFrameInfo_.frameInfo.Channels));
        if(!cap.read(frame))    // nothing to read
            break;
        writer.write(frame);
    }

    cap.release();
    writer.release();
}

std::string RTSPCamera::getRecvFilename() const
{
    return m_recvFilename;
}

void RTSPCamera::setRecvFilename(const std::string &recvFilename)
{
    m_recvFilename = recvFilename;
}
