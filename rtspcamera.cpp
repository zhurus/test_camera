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

    VideoCapture cap;
    if(!cap.open(m_url))
        throw std::runtime_error("RTSPCamera::Start: Failed to connect input video");

    double frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    double frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);

    VideoWriter writer(
                    m_recvFilename,           // output filename
                    writer.fourcc('A','V','C','1'), // FFMPEG Codec
                    fps,
                    Size2d(frame_width,frame_height)
                );

    // Reading input and recording to output
    Mat frame;
    while(true) {
        Mat frame;
        if(!cap.read(frame))
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
