#include <iostream>

#include <thread>
#include <future>
#include <gio/gio.h>

#include <gio/gunixfdlist.h>
#include "cameradbusinterface.h"

#include "complexdbusobject.h"
#include "rtspcamera.h"
#include "dbusClient.h"

#include <boost/program_options.hpp>




int main(int argc, char** argv)
{
    namespace po = boost::program_options;

    //Parameters for getting from command line
    std::string bus;
    std::string grid;
    std::string url;

    try {
        po::options_description desc("Options");
        desc.add_options()
                ("help,h","Help")
                ("bus,b",po::value<std::string>(&bus)->default_value(""),"D-Bus messages")
                ("grid,g",po::value<std::string>(&grid)->default_value(""),"Unique ID")
                ("url,u",po::value<std::string>(&url)->default_value(""),"Videostream source");
        po::variables_map vm;
        po::parsed_options parsed =
                po::command_line_parser(argc, argv).options(desc)
                .allow_unregistered().run();
        po::store(parsed,vm);
        po::notify(vm);
        bus = vm["bus"].as<std::string>();
        grid= vm["grid"].as<std::string>();
        url = vm["url"].as<std::string>();

        if(vm.count("help")) {
            std::cout<<desc<<std::endl;
            return 0;
        }
    } catch (po::error e) {
        std::cerr<<e.what()<<std::endl;
    }

//    //Creation of dbus connection
//    dbusClient client(bus);

    //Camera creation
    ICamera *ptr = new RTSPCamera(url);

    //start camera main cycle
    auto fut = std::async(std::launch::async,[ptr](){(ptr)->Start();});

    //create camera dbus object
    Camera cam(ptr);

//    guint owner_id;
//    GMainLoop *loop;
//    ComplexDbusObject service(bus);

//    loop = g_main_loop_new (NULL, FALSE);


//    //start application
//    g_main_loop_run (loop);

//    g_bus_unown_name (service.getOwner_id());

////    g_dbus_node_info_unref (introspection_data);
    return 0;
}
