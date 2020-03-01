#ifndef RTSPCAMERA_H
#define RTSPCAMERA_H

#include "icamera.h"

#include <string>




class RTSPCamera: public ICamera
{
public:
    RTSPCamera(const std::string& url,
               const std::string& getRecvFilename = "received.mp4");
    virtual ~RTSPCamera() override;

    void Start() override;

    std::string getRecvFilename() const;
    void setRecvFilename(const std::string &getRecvFilename);

private:
    std::string m_url;
    std::string m_recvFilename;
};

#endif // RTSPCAMERA_H
