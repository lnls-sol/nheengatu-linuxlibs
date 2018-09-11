#######################################
# Project: CrioLinux
#######################################
cmake_minimum_required(VERSION 2.6)
#cmake_policy(VERSION 3.4)

project(CrioLinux)
enable_testing()

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
add_definitions(-DBUILD_CRIO_LINUX_LIBDIR=\"${CRIO_LINUX_LIBDIR}/\")

########################################
# External modules
########################################
find_package(Boost 1.35.0 REQUIRED)
check_module_no_pkg(NIFPGAHELPER NiFpga.h NiFpgaHelper)
#find_path(NIFPGA_CRIOLINUX_INCLUDE_DIR NiFpga_CrioLinux.h HINTS ENV CPATH ENV C_INCLUDE_PATH ${CRIO_LINUX_LIBDIR})
find_package (Threads)

########################################
# Compiler flags
########################################
add_compiler_flags(-Wall -Wextra -Werror -Wmissing-declarations -Wno-maybe-uninitialized
    -Wno-missing-field-initializers -Wno-unused-parameter -fdata-sections -ffunction-sections -g)
add_linker_flags(-Wl,--as-needed -Wl,--gc-sections)
add_double_flags(-fsanitize=undefined)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_double_flags(-flto)

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_linker_flags(-s)
    endif()
endif()

########################################
# Include files
########################################
include_directories(include)
include_directories(${NIFPGAHELPER_INCLUDE_DIRS})
include_directories(${NIFPGA_CRIOLINUX_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS})

########################################
# Main target: CrioLinux
########################################
file(GLOB LIB_SOURCE "src/*.cc")
add_library(CrioLinux SHARED ${LIB_SOURCE})
target_link_libraries(CrioLinux ${NIFPGAHELPER_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lrt)
set_target_properties(CrioLinux PROPERTIES VERSION 0.1.0 SOVERSION 0)
set_target_properties(CrioLinux PROPERTIES CXX_VISIBILITY_PRESET hidden)

########################################
# Installation
########################################
install(TARGETS CrioLinux DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES include/CrioLinux.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


########################################
# Evals
########################################
#add_eval(readTime eval/readSpeed.cc)
#add_eval(sharedMutex eval/mutex.cc eval/shared_mutex.cc)

########################################
# Tests
########################################

add_simple_test(TestBasicBO tests/BasicBO.cc)
add_simple_test(TestBasicBI tests/BasicBI.cc)
add_simple_test(TestBasicSetup tests/BasicSetup.cc)

########################################
# Final log
########################################
message("")
message("===============================================================================")
message("")
show_target(CrioLinux)
message("")
message("===============================================================================")
message("")
