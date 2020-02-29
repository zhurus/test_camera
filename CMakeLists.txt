cmake_minimum_required(VERSION 2.8)

project(test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0015 NEW)

file(GLOB SOURCES
	*.h
	*.cpp
    )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})


find_package(Boost 1.65.0 REQUIRED program_options)
include_directories(${Boost_INCLUDE_DIRS})


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	${GTK3_LIBRARIES}
	${Boost_LIBRARIES}
	)
