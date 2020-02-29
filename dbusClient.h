#pragma once

#include <gio/gio.h>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#define p(x) std::cout<<#x<<"---------"<<(x)<<std::endl
#define ps(x) g_print("[%s]=%s \n",#x, (x))
#define pi(x) g_print("[%s]=%d \n",#x, (x))
#define var(x)  #x


#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
            (
            #ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                    nullptr, nullptr),
            #else
                nullptr,
            #endif
                std::free
                );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}



typedef struct HostInfo
{
    std::string name;
    std::string APP;
    int state;
    std::string groupUID;
    std::string hostUID;
    int pid;
    int sharedMemoryID;
    //std::map<std::string,std::string> comparator;

    HostInfo()
    {
        state=-1;
        pid=-1;
        sharedMemoryID=-1;
    }

    void  print()
    {
        std::cout<<"||||||||||||||||||||||||||||\n";
        p(name);
        p(APP);
        p(state);
        p(groupUID);
        p(hostUID);
        p( pid);
        p(sharedMemoryID);
        std::cout<<"||||||||||||||||||||||||||||\n";
    };
    void compareMap(std::map<std::string,std::string> &comp2)
    {
        for (std::map<std::string,std::string>::iterator it=comp2.begin(); it!=comp2.end(); ++it)
        {
            if( strcmp("APP",it->first.c_str()   )==0 )//  << " => " << it->second << '\n';
            {
                APP=it->second;
            }
            else if(strcmp("state",it->first.c_str()   )==0  )
            {
                state=std::stoi(it->second);
            }
            else if(strcmp("pid",it->first.c_str()   )==0  )
            {
                pid=std::stoi(it->second);
            }
            else if(strcmp("sharedMemoryID",it->first.c_str()   )==0  )
            {
                sharedMemoryID=std::stoi(it->second);
            }
            else if(strcmp("name",it->first.c_str()   )==0  )
            {
                name= it->second ;
            }
            else if(strcmp("groupUID",it->first.c_str()   )==0  )
            {
                groupUID= it->second ;
            }
            else if(strcmp("hostUID",it->first.c_str()   )==0  )
            {
                hostUID= it->second ;
            }
        }
    }

}HostInfo;


typedef struct GroupInfo
{

    gboolean isLocked;
    guint64 sharedMemoryID;
    std::string groupUID;

    std::vector<HostInfo> Hosts;

    std::map<std::string,std::string> comparator;

    GroupInfo()
    {
        comparator.insert({var(isLocked),"b"});
        comparator.insert({var(sharedMemoryID),"t"});
        comparator.insert({var(groupUID),"x"});
    }

    void  print()
    {
        p(isLocked);
        p(sharedMemoryID);
        p(groupUID);
        for (int i=0;i<Hosts.size();++i)
        {
            Hosts[i].print();
        }
    }
}GroupInfo;


struct Data
{

    gint64  a;
    gint64  b;
    guint64  c;
    guint64  d;
    Data()
    {
        this->a=0;
        this->b=0;
        this->c=0;
        this->d=0;
    }
    void data_print()
    {
        g_print("data.a: %ld \n",this->a);
        g_print("data.b: %ld \n",this->b);
        g_print("data.c: %lu \n",this->c);
        g_print("data.d: %lu \n",this->d);
    }


};



class dbusClient
{
public:
    dbusClient(std::string busName);
    ~dbusClient();
    //org.korda.cortes.Launcher
    // /org/korda/cortes/Launcher/Launcher
    //org.korda.cortes.Launcher
    static void getGroupInfo(GroupInfo *d,std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4");
    static int getPID();
    static gboolean getGroupLock(std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4");

    static HostInfo getHostInfo(std::string HostUID="34871b1a-0089-4d79-8842-132618ebc144");  //this method return nothing!!!!
    static std::vector<HostInfo> getHostInfoList();
    static std::string getHostUID(gint32 pid);
    static void HostStateChanged(std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4", int status=29948);
    static void setGroupLock(std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4",gboolean isLocked=0);
    static void switchGroupLock(std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4");
    static std::string registerHost(int pid=29948,std::string groupUID="2e7df986-9c01-11e9-a2a3-2a2ae2dbcce4");


    //org.korda.cortes.Launcher
    // /org/korda/cortes/Launcher/FramebufferProvider
    //org.korda.cortes.FramebufferProvider

    static gint32 CreateFramebuffer (std::string anyUID="any", guint64 frameBufferSize=1024, guint64 chunkSize=256); // (Int32 sharedMemoryID)
    static void DestroyFramebuffer (std::string  anyUID="any", gint32 sharedMemoryID=0);
    static gint32 GetFramebuffer (std::string anyUID="any");// (Int32 sharedMemoryID)
    static std::map<std::string,std::string> GetFramebufferList ();// ↦ (Dict of {String, Variant}

    static GDBusConnection * getBusConnection();
private:
    static GDBusConnection *connection;



};
