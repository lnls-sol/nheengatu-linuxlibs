#######################################
# Project: CrioLinux
#######################################
cmake_minimum_required(VERSION 2.6)
cmake_policy(VERSION 3.4)

project(CrioLinux)

########################################
# Build includes
#######################################
include(GNUInstallDirs)
include(macros.cmake)

#######################################
# Global setup
#######################################
set_default_build_type(RelWithDebInfo)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CRIO_LINUX_LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR}/crio-linux-libs CACHE STRING
    "Location of cRIO Linux LabView FPGA generated files")

########################################
# External modules
########################################
check_module_no_pkg(NIFPGAHELPER NiFpga.h NiFpgaHelper)
find_path(NIFPGA_CRIOLINUX_INCLUDE_DIR NiFpga_CrioLinux.h HINTS ENV CPATH ENV C_INCLUDE_PATH
    ${CRIO_LINUX_LIBDIR})

########################################
# Include files
########################################
include_directories(include)
include_directories(${NIFPGAHELPER_INCLUDE_DIRS})
include_directories(${NIFPGA_CRIOLINUX_INCLUDE_DIR})

########################################
# Main target: CrioLinux
########################################
file(GLOB LIB_SOURCE "src/*.cc")
add_library(CrioLinux SHARED ${LIB_SOURCE})
target_link_libraries(CrioLinux ${NIFPGAHELPER_LIBRARIES})
set_target_properties(CrioLinux PROPERTIES VERSION 0.1.0 SOVERSION 0)

########################################
# Installation
########################################
install(TARGETS CrioLinux DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/CrioLinux.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
