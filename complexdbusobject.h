#ifndef COMPLEXDBUSOBJECT_H
#define COMPLEXDBUSOBJECT_H
#include <gio/gio.h>
#include "cameradbusinterface.h"
#include "dbusClient.h"


class ComplexDbusObject
{
public:
    static gchar **
    subtree_enumerate (GDBusConnection       *connection,
                       const gchar           *sender,
                       const gchar           *object_path,
                       gpointer               user_data);

    static GDBusInterfaceInfo **
    subtree_introspect (GDBusConnection       *connection,
                        const gchar           *sender,
                        const gchar           *object_path,
                        const gchar           *node,
                        gpointer               user_data);


    static const GDBusInterfaceVTable *
    subtree_dispatch (GDBusConnection             *connection,
                      const gchar                 *sender,
                      const gchar                 *object_path,
                      const gchar                 *interface_name,
                      const gchar                 *node,
                      gpointer                    *out_user_data,
                      gpointer                     user_data);

    static void
    on_bus_acquired (GDBusConnection *connection,
                     const gchar     *name,
                     gpointer         user_data);

    static void
    on_name_acquired (GDBusConnection *connection,
                      const gchar     *name,
                      gpointer         user_data);

    static void
    on_name_lost (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data);
    ComplexDbusObject(std::string bus);
    static GDBusSubtreeVTable subtree_vtable;
    int getOwner_id() const;

private:
    int owner_id;
};

#endif // COMPLEXDBUSOBJECT_H
