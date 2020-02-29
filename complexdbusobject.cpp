#include "complexdbusobject.h"
#include "cameradbusinterface.h"

GDBusSubtreeVTable ComplexDbusObject::subtree_vtable =
{
    subtree_enumerate,
    subtree_introspect,
    subtree_dispatch
};
gchar **ComplexDbusObject::subtree_enumerate(GDBusConnection *connection, const gchar *sender, const gchar *object_path, gpointer user_data)
{
    gchar **nodes;
    GPtrArray *p;

    p = g_ptr_array_new ();
    if(Camera::cameraInterface)
        g_ptr_array_add (p, g_strdup ("FrameProvider"));

    g_ptr_array_add (p,NULL);
    nodes = (gchar **) g_ptr_array_free (p, FALSE);

    return nodes;
}

GDBusInterfaceInfo **ComplexDbusObject::subtree_introspect(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *node, gpointer user_data)
{
    GPtrArray *p;
    p = g_ptr_array_new ();
    if (node == NULL)
    {
    }
    else
    {
        if(!g_strcmp0("FrameProvider",node)){
            if(Camera::cameraInterface)
                g_ptr_array_add (p, g_dbus_interface_info_ref (Camera::cameraInterface));

    }

    g_ptr_array_add (p, NULL);

    return (GDBusInterfaceInfo **) g_ptr_array_free (p, FALSE);
}
}

const GDBusInterfaceVTable *ComplexDbusObject::subtree_dispatch(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *node, gpointer *out_user_data, gpointer user_data)
{
    const GDBusInterfaceVTable *vtable_to_return;
    gpointer user_data_to_return;

    if (g_strcmp0 (interface_name, "org.korda.cortes.FrameProvider") == 0)
        vtable_to_return = &Camera::interface_vtable;
    else
        g_assert_not_reached ();

    *out_user_data = user_data_to_return;

    return vtable_to_return;
}

void ComplexDbusObject::on_bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    guint registration_id;

    registration_id = g_dbus_connection_register_subtree (connection,
                                                          "/org/korda/cortes",
                                                          &subtree_vtable,
                                                          G_DBUS_SUBTREE_FLAGS_NONE,
                                                          NULL,  /* user_data */
                                                          NULL,  /* user_data_free_func */
                                                          NULL); /* GError** */
    g_assert (registration_id > 0);
}

void ComplexDbusObject::on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{

}

void ComplexDbusObject::on_name_lost(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    exit (1);
}

ComplexDbusObject::ComplexDbusObject(std::string bus)
{
    std::string path = std::string("org.korda.cortes.FrameProvider.instance")+std::to_string(getpid());
    owner_id = g_bus_own_name_on_connection(dbusClient::getBusConnection(),
                                            path.c_str(),
                                            G_BUS_NAME_OWNER_FLAGS_REPLACE,
                                            on_bus_acquired,
                                            on_name_lost,
                                            NULL,
                                            NULL);
}

int ComplexDbusObject::getOwner_id() const
{
    return owner_id;
}
