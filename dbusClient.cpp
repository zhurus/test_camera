#include "dbusClient.h"
#include <map>
GDBusConnection *dbusClient::connection =NULL;

///----------------------helper functions start
std::string getStringVal(GVariant * var)
{
    gchar* key;
    GVariant * groupUID_v_value;

    g_variant_get ( var, "{sv}", & key,&groupUID_v_value);

    gchar* val;
    g_variant_get ( groupUID_v_value, "s", &val);
    std::string str=val;

    g_variant_unref (groupUID_v_value);
    g_variant_unref (var);
    printf("\n key:  %s ----------- value:\n%s \n ",key,str.c_str());
    // g_free(key);
    // g_free(val);

    return str;
}

guint64 getUint64Val(GVariant * var)
{
    gchar* key;
    GVariant * groupUID_v_value;

    g_variant_get ( var, "{sv}", & key,&groupUID_v_value);

    guint64 val;
    g_variant_get ( groupUID_v_value, "x", &val);


    g_variant_unref (groupUID_v_value);
    g_variant_unref (var);
    g_print("\n key:%s value:%lu\n",key,val);
    g_free(key);


    return val;
}

gint64 getInt64Val(GVariant * var)
{
    gchar* key;
    GVariant * groupUID_v_value;

    g_variant_get ( var, "{sv}", & key,&groupUID_v_value);

    guint64 val;
    g_variant_get ( groupUID_v_value, "x", &val);


    g_variant_unref (groupUID_v_value);
    g_variant_unref (var);
    g_print("\n key:%s value:%l\n",key,val);
    g_free(key);


    return val;
}

gboolean getGbooleanVal(GVariant * var)
{
    gchar* key;
    GVariant * groupUID_v_value;

    g_variant_get ( var, "{sv}", & key,&groupUID_v_value);

    gboolean val;
    g_variant_get ( groupUID_v_value, "b", &val);


    g_variant_unref (groupUID_v_value);
    g_variant_unref (var);
    g_print("\n key:%s value:%d\n",key,val);
    g_free(key);


    return val;
}

HostInfo getHostInfoVal(GVariant * var)
{
    HostInfo h;

    std::map<std::string,std::string> comp2;
    try
    {
        printf("getHostInfo Childs in variant c2:%ld \n",g_variant_n_children(var) );
        for(unsigned int  i=0; i<g_variant_n_children(var);++i)
        {
            GVariant * val;
            val=g_variant_get_child_value (var,i);
            //val=g_variant_get_child_value (val,1);
            p(i);
            // printf("++getHostInfo Childs in variant c2:%ld \n",g_variant_n_children(val) );

            GVariant * val2;
            gchar* key;
            g_variant_get(val,"{sv}",&key,&val2);
            ps(key);
            const gchar* val_type;
            val_type= g_variant_get_type_string(val2);

            std::string V;

            if( strcmp (val_type,"s")==0)
            {
                const gchar* val_string;
                g_variant_get(val2,"s", &val_string);
                ps(val_string);
                V= val_string ;

            }
            else if( strcmp (val_type,"x")==0)
            {
                int val_int;
                g_variant_get(val2,"x", &val_int);
                pi(val_int);
                V=std::to_string(val_int);
            }

            comp2.insert({std::string(key),V});
        }
        h.compareMap(comp2);



    }
    catch (...)
    {
        printf("getHostInfoVal \n" );
    }



    return h;
}


HostInfo getHostInfoVal_2(GVariant * var)
{
    HostInfo h;

    std::map<std::string,std::string> map;

    try
    {
        GVariant * c2=var;

        printf("Childs in variant c2:%ld \n",g_variant_n_children(var) );
        c2=g_variant_get_child_value (var,0);

        gchar* key;
        GVariant * val;
        g_variant_get(c2,"{sv}",&key,&val);

        g_variant_unref (var);
        g_variant_unref (c2);


        ps(key);

    } catch (...)
    {
        printf("getHostInfoVal =null \n" );
    }



    return h;
}


///----------------------helper functions end
dbusClient::dbusClient(std::string busName)
{
    GError *error = NULL;

    //подключаемся к кастомной шине
    connection = g_dbus_connection_new_for_address_sync (busName.c_str(),
                                                         (GDBusConnectionFlags)(G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT |
                                                                                G_DBUS_CONNECTION_FLAGS_MESSAGE_BUS_CONNECTION)    ,
                                                         NULL, /* GDBusAuthObserver */
                                                         NULL, /* GCancellable */
                                                         &error);
    if (connection == NULL)
    {
        g_printerr ("Error connecting to D-Bus address %s\n",busName.c_str());
    }
}

dbusClient::~dbusClient()
{
//    g_bus_unown_name(name_owned);
    g_dbus_connection_close(connection,NULL,NULL,NULL);
}



int dbusClient::getPID()
{
    std::vector<HostInfo> inf=getHostInfoList();


    return inf.at(0).pid;
}

gboolean dbusClient::getGroupLock(std::string groupUID)
{
    GError *errorCall = NULL;
    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "GetGroupLock",          /* method name */
                                                      g_variant_new("(s)",groupUID.c_str()  ),
                                                      G_VARIANT_TYPE ("(b)"),
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);

    gboolean b;
    g_variant_get (HostInfo, "(b)", &b);


    return b;
}

HostInfo dbusClient::getHostInfo(std::string HostUID)
{
    GError *errorCall = NULL;
    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "GetHostInfo",          /* method name */
                                                      g_variant_new("(s)",HostUID.c_str()  ),
                                                      G_VARIANT_TYPE ("(v)"),
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);


    GVariant * get =g_variant_get_child_value (HostInfo,0);

    get =g_variant_get_child_value (get,0);
    // get =g_variant_get_child_value (get,0);

    GVariant* b;
    g_variant_get (HostInfo, "(v)", &b);
    struct HostInfo inf;


    try
    {
        inf=getHostInfoVal(HostInfo );
    } catch (...)
    {
        g_print("empty HostInfo: %s",&b);
    }

    return  inf;

}

std::vector<HostInfo> dbusClient::getHostInfoList()
{
    std::vector<HostInfo> hosts;

    GError *errorCall = NULL;
    GVariant *HostInfo2 = g_dbus_connection_call_sync (connection,
                                                       "org.korda.cortes.Launcher",  /* bus name */
                                                       "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                       "org.korda.cortes.Launcher",  /* interface name */
                                                       "GetHostInfoList",          /* method name */
                                                       NULL,
                                                       G_VARIANT_TYPE_TUPLE ,
                                                       G_DBUS_CALL_FLAGS_NONE,
                                                       -1,
                                                       NULL,
                                                       &errorCall);

    GVariant *HostInfo;


    for(uint i=0 ;i<(uint)g_variant_n_children(HostInfo2) ;++i)
    {
        HostInfo=g_variant_get_child_value (HostInfo2,0);

        // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
        HostInfo=g_variant_get_child_value (HostInfo,0);
        // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
        HostInfo=g_variant_get_child_value (HostInfo,1);
        // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
        HostInfo=g_variant_get_child_value (HostInfo,0);
        // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
        HostInfo=g_variant_get_child_value (HostInfo,0);
        // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );


        struct HostInfo hi;
        hi=getHostInfoVal(HostInfo);
        hosts.push_back(hi);
    }


    //    HostInfo=g_variant_get_child_value (HostInfo2,0);

    //    // printf("+1Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
    //    HostInfo=g_variant_get_child_value (HostInfo,1);
    //    // printf("+2Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
    //    HostInfo=g_variant_get_child_value (HostInfo,1);
    //    // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
    //    HostInfo=g_variant_get_child_value (HostInfo,0);
    //    // printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );
    //    HostInfo=g_variant_get_child_value (HostInfo,0);
    //    //  printf("Childs in variant:%ld \n",g_variant_n_children(HostInfo) );

    //    struct HostInfo hi2;
    //    hi2=getHostInfoVal(HostInfo);
    //    hosts.push_back(hi2);


    return hosts;
}

std::string dbusClient::getHostUID(gint32 pid)
{
    GError *errorCall = NULL;

    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "GetHostUID",          /* method name */
                                                      g_variant_new("(i)",pid  ),
                                                      G_VARIANT_TYPE ("(s)"),
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      1500,
                                                      NULL,
                                                      &errorCall);

    if(HostInfo==NULL)
    {
        g_error_free (errorCall);
    }


    gchar* getter;
    g_variant_get (HostInfo, "(s)", &getter);

    return std::string(getter);


}

void dbusClient::HostStateChanged(std::string groupUID, int status)
{
    GError *errorCall = NULL;

    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "HostStateChanged",          /* method name */
                                                      g_variant_new("(si)", groupUID.c_str() ,status),
                                                      NULL,
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);
}

void dbusClient::setGroupLock(std::string groupUID, gboolean isLocked)
{
    GError *errorCall = NULL;

    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "SetGroupLock",          /* method name */
                                                      g_variant_new("(sb)", groupUID.c_str() ,isLocked),
                                                      NULL,
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);
}

void dbusClient::switchGroupLock(std::string groupUID)
{
    GError *errorCall = NULL;

    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "SwitchGroupLock",          /* method name */
                                                      g_variant_new("(s)",  groupUID.c_str() ),
                                                      NULL,
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);
}

std::string dbusClient::registerHost(int pid, std::string groupUID)
{
    GError *errorCall = NULL;

    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "registerHost",          /* method name */
                                                      g_variant_new("(is)",pid, groupUID.c_str() ),
                                                      G_VARIANT_TYPE ("(s)"),
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);




    if(HostInfo==NULL)
    {
        g_error_free (errorCall);


    }


    //        const gchar *name_error;
    //        g_print("error code :%d \n",errorCall->code);
    //        name_error=g_quark_to_string (errorCall->domain);
    //        g_print("error domain :%s \n",name_error);
    //    }

    gchar* getter;
    g_variant_get (HostInfo, "(s)", &getter);

    return std::string(getter);
}

gint32 dbusClient::CreateFramebuffer(std::string anyUID, guint64 frameBufferSize, guint64 chunkSize)
{

    //org.korda.cortes.Launcher
    // /org/korda/cortes/Launcher/FramebufferProvider
    //org.korda.cortes.FramebufferProvider
    GError *errorCall = NULL;
//    auto timeStart(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    GVariant* result = g_dbus_connection_call_sync (connection,
                                                    "org.korda.cortes.Launcher",  /* bus name */
                                                    "/org/korda/cortes/Launcher/FramebufferProvider", /* object path */
                                                    "org.korda.cortes.FramebufferProvider",  /* interface name */
                                                    "CreateFramebuffer",          /* method name */
                                                    g_variant_new("(stt)",anyUID.c_str(),frameBufferSize,chunkSize),
                                                    G_VARIANT_TYPE ("(i)"),
                                                    G_DBUS_CALL_FLAGS_NONE,
                                                    -1,
                                                    NULL,
                                                    &errorCall);
//    auto timeEnd(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());
//    LOGGER_INFO(std::to_string(timeEnd-timeStart),0,{});
    gint32 getter;
    g_variant_get (result, "(i)", &getter);
    return getter;
}

void dbusClient::DestroyFramebuffer(std::string anyUID, gint32 sharedMemoryID)
{
    GError *errorCall = NULL;
    GVariant* result = g_dbus_connection_call_sync (connection,
                                                    "org.korda.cortes.Launcher",  /* bus name */
                                                    "/org/korda/cortes/Launcher/FramebufferProvider", /* object path */
                                                    "org.korda.cortes.FramebufferProvider",  /* interface name */
                                                    "DestroyFramebuffer",          /* method name */
                                                    g_variant_new("(st)",anyUID.c_str(),sharedMemoryID),
                                                    NULL,
                                                    G_DBUS_CALL_FLAGS_NONE,
                                                    10,
                                                    NULL,
                                                    &errorCall);
}

gint32 dbusClient::GetFramebuffer(std::string anyUID)
{
    GError *errorCall = NULL;
    GVariant* result = g_dbus_connection_call_sync (connection,
                                                    "org.korda.cortes.Launcher",  /* bus name */
                                                    "/org/korda/cortes/Launcher/FramebufferProvider", /* object path */
                                                    "org.korda.cortes.FramebufferProvider",  /* interface name */
                                                    "GetFramebuffer",          /* method name */
                                                    g_variant_new("(s)",anyUID.c_str() ),
                                                    G_VARIANT_TYPE ("(i)"),
                                                    G_DBUS_CALL_FLAGS_NONE,
                                                    10,
                                                    NULL,
                                                    &errorCall);

    gint32 getter;
    g_variant_get (result, "(i)", &getter);
    return getter;
}

std::map<std::string, std::string> dbusClient::GetFramebufferList()
{
    GError *errorCall = NULL;
    GVariant* result = g_dbus_connection_call_sync (connection,
                                                    "org.korda.cortes.Launcher",  /* bus name */
                                                    "/org/korda/cortes/Launcher/FramebufferProvider", /* object path */
                                                    "org.korda.cortes.FramebufferProvider",  /* interface name */
                                                    "GetFramebuffer",          /* method name */
                                                    NULL,
                                                    G_VARIANT_TYPE ("a{sv}"),
                                                    G_DBUS_CALL_FLAGS_NONE,
                                                    10,
                                                    NULL,
                                                    &errorCall);


std::map<std::string, std::string> m;

return m;
}







void dbusClient::getGroupInfo(GroupInfo *d,std::string groupUID)
{

    GError *errorCall = NULL;
    GVariant *HostInfo = g_dbus_connection_call_sync (connection,
                                                      "org.korda.cortes.Launcher",  /* bus name */
                                                      "/org/korda/cortes/Launcher/Launcher", /* object path */
                                                      "org.korda.cortes.Launcher",  /* interface name */
                                                      "GetGroupInfo",          /* method name */
                                                      g_variant_new("(s)",groupUID.c_str()  ),
                                                      G_VARIANT_TYPE ("(v)"),
                                                      G_DBUS_CALL_FLAGS_NONE,
                                                      -1,
                                                      NULL,
                                                      &errorCall);

    GVariant * result_inner;
    g_variant_get (HostInfo, "(v)", &result_inner);


    printf("Childs in variant:%ld \n",g_variant_n_children(result_inner) );

    GVariant * result_inner2=g_variant_get_child_value (result_inner,0);




    d->groupUID= getStringVal(result_inner2);
    result_inner2=g_variant_get_child_value (result_inner,1);
    d->sharedMemoryID=  getUint64Val(result_inner2);
    result_inner2=g_variant_get_child_value (result_inner,2);
    d->isLocked=getGbooleanVal(result_inner2);
    result_inner2=g_variant_get_child_value (result_inner,3);


    result_inner2=g_variant_get_child_value (result_inner2,1);



    int n=g_variant_n_children(result_inner2);
    printf("avaiable hosts in variant:%ld \n",g_variant_n_children(result_inner2));

    for(int  i=0;i< n  ;++i)
    {
        result_inner2=g_variant_get_child_value (result_inner2,i);
        struct HostInfo inf=getHostInfoVal(result_inner2);

        d->Hosts.push_back(inf);
    }
    //g_print("\n key:%s ",c);

    // printf("Childs in variant:%ld \n",g_variant_n_children(c) );


    g_variant_unref (HostInfo);
    g_variant_unref (result_inner);
}




GDBusConnection *dbusClient::getBusConnection()
{
    return connection;
}

