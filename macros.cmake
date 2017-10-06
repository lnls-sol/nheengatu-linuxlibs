function(set_default_build_type BUILD_TYPE)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting default build type to '${BUILD_TYPE}'.")
        set(CMAKE_BUILD_TYPE "${BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                "MinSizeRel" "RelWithDebInfo")
    endif()
endfunction()

function(check_module_no_pkg PREFIX INC_NAME LIB_NAME)
    find_library(${PREFIX}_LIBRARIES ${LIB_NAME} HINTS ENV LIBRARY_PATH)
    find_path(${PREFIX}_INCLUDE_DIRS ${INC_NAME} HINTS ENV CPATH ENV C_INCLUDE_PATH)
endfunction()
