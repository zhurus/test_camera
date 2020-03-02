#ifndef BASECAMERA_H
#define BASECAMERA_H
#include <iostream>
#include <gio/gio.h>
#include "icamera.h"


struct CameraDbusInterface
{
    std::string xml =
            "<node>"
            " <interface name='org.korda.cortes.FrameProvider'>"
            "    <signal name='FrameReceived'>"
            "    <arg name='FrameID' type='t' direction='out'/>"
            "    <arg name='Timestamp' type='t' direction='out'/>"
            "    </signal>"
            "    <method name='GetLastFrame'>"
            "    <arg name='FrameID' type='v' direction='out'/>"
            "    </method>"

            "     <method name='Start'/>"
            "    <method name='Stop'/>"

            "     <property name='Height' type='i' access='readwrite' />"
            "     <property name='Width' type='i' access='readwrite' />"
            "    <property name='Channels' type='i' access='readwrite' />"
            "     <property name='PixelFormat' type='s' access='read' />"
            "    <property name='Name' type='s' access='read' />"
            "     <property name='Vendor' type='s' access='read' />"
            "    <property name='Type' type='s' access='read' />"
            "     <property name='SMID' type='t' access='read' />"
            " </interface>"
            "</node>";
    GDBusNodeInfo *introspection_data = NULL;
};


class Camera
{
public:
    Camera(ICamera *cam);
    ~Camera();
    static void
    handle_method_call (GDBusConnection       *connection,
                        const gchar           *sender,
                        const gchar           *object_path,
                        const gchar           *interface_name,
                        const gchar           *method_name,
                        GVariant              *parameters,
                        GDBusMethodInvocation *invocation,
                        gpointer               user_data);



    static CameraDbusInterface getInterface();
    static GDBusInterfaceVTable interface_vtable ;
    static CameraDbusInterface interface;
    static GDBusInterfaceInfo *cameraInterface;

    void setHeight(const int height);
    void setWeight(const int width);
    void setChannels(const int channels);

    int getPixelFormat()const;
    std::string getName()const;
    std::string getVendor()const;
    std::string getType()const;
    int getSMID()const;


private:
    static ICamera* camera;

};


#endif // BASECAMERA_H
