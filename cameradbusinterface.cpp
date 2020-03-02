#include "cameradbusinterface.h"

CameraDbusInterface Camera::interface;
GDBusInterfaceInfo *Camera::cameraInterface = NULL;
GDBusInterfaceVTable Camera::interface_vtable =
{
    Camera::handle_method_call

};
ICamera* Camera::camera = NULL;
Camera::Camera( ICamera *cam)
{
    Camera::interface.introspection_data = g_dbus_node_info_new_for_xml (Camera::interface.xml.c_str(), NULL);
    cameraInterface = g_dbus_node_info_lookup_interface (Camera::interface.introspection_data, "org.korda.cortes.FrameProvider");
    Camera::camera = cam;

}

Camera::~Camera()
{
    g_dbus_node_info_unref (Camera::interface.introspection_data);

}

void Camera::handle_method_call(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *method_name, GVariant *parameters, GDBusMethodInvocation *invocation, gpointer user_data)
{
    if (g_strcmp0 (method_name, "Start") == 0)
    {
        camera->Start();
        g_dbus_method_invocation_return_value (invocation, NULL);

    }
    else if (g_strcmp0 (method_name, "Stop") == 0)
    {
        camera->Stop();
        g_dbus_method_invocation_return_value (invocation, NULL);

    }
    else if (g_strcmp0 (method_name, "GetLastFrame") == 0)
    {
        GVariant *range[4];
        CurrentFrameInfo currentFrameInfo_ = camera->currentFrameInfo();
        range[0] = g_variant_new_int32(currentFrameInfo_.frameInfo.Height);
        range[1] = g_variant_new_int32(currentFrameInfo_.frameInfo.Width);
        range[2] = g_variant_new_uint64(currentFrameInfo_.frameNum);
        range[3] = g_variant_new_uint64(currentFrameInfo_.timeStamp);

        g_dbus_method_invocation_return_value(invocation,g_variant_new("(v)",g_variant_new_tuple(range, 4)));
        return;

    }
}




CameraDbusInterface Camera::getInterface()
{
    return interface;
}

void Camera::setHeight(const int height)
{
    auto fi = camera->currentFrameInfo();
    fi.frameInfo.Height = height;
    camera->setCurrentFrameInfo(fi);
}

void Camera::setWeight(const int width)
{
    auto fi = camera->currentFrameInfo();
    fi.frameInfo.Width = width;
    camera->setCurrentFrameInfo(fi);
}

void Camera::setChannels(const int channels)
{
    auto fi = camera->currentFrameInfo();
    fi.frameInfo.Channels = channels;
    camera->setCurrentFrameInfo(fi);
}

int Camera::getPixelFormat() const
{
    return camera->currentFrameInfo().frameInfo.PixelFormat;
}

std::string Camera::getName() const
{
    return camera->getCameraInfo().name;
}

std::string Camera::getVendor() const
{
    return camera->getCameraInfo().vendor;
}

std::string Camera::getType() const
{
    return camera->getCameraInfo().type;
}

int Camera::getSMID() const
{
    return camera->getCameraInfo().shmid;
}
