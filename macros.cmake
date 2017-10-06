include(CheckCXXCompilerFlag)

function(set_default_build_type BUILD_TYPE)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting default build type to '${BUILD_TYPE}'.")
        set(CMAKE_BUILD_TYPE "${BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                "MinSizeRel" "RelWithDebInfo")
    endif()
endfunction()

function(add_compiler_flags)
    foreach(COMPILER_FLAG ${ARGV})
        string(REGEX REPLACE "[+=-]" "_" ESCAPED ${COMPILER_FLAG})
        check_cxx_compiler_flag(${COMPILER_FLAG} HAVE_CXX_${ESCAPED})

        if (${HAVE_CXX_${ESCAPED}})
            add_compile_options(${COMPILER_FLAG})
        endif()
    endforeach(COMPILER_FLAG)
endfunction()

function(add_double_flags)
    foreach(DOUBLE_FLAG ${ARGV})
        string(REGEX REPLACE "[+=-]" "_" ESCAPED ${DOUBLE_FLAG})
        set(CMAKE_REQUIRED_FLAGS ${DOUBLE_FLAG})
        check_cxx_compiler_flag("" HAVE_CXX_${ESCAPED})

        if (${HAVE_CXX_${ESCAPED}})
            set(CXX_FLAGS "${CXX_FLAGS} ${DOUBLE_FLAG}")
        endif()
    endforeach(DOUBLE_FLAG)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${CXX_FLAGS}" PARENT_SCOPE)
endfunction()

function(add_linker_flags)
    foreach(LINKER_FLAG ${ARGV})
        string(REGEX REPLACE "[+=-]" "_" ESCAPED ${LINKER_FLAG})
        set(CMAKE_REQUIRED_FLAGS ${LINKER_FLAG})
        check_cxx_compiler_flag("" HAVE_${ESCAPED})

        if (${HAVE_${ESCAPED}})
            set(FLAGS "${FLAGS} ${LINKER_FLAG}")
        endif(${HAVE_${ESCAPED}})
    endforeach(LINKER_FLAG)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}${FLAGS}" PARENT_SCOPE)
endfunction()

function(check_module_no_pkg PREFIX INC_NAME LIB_NAME)
    find_library(${PREFIX}_LIBRARIES ${LIB_NAME} HINTS ENV LIBRARY_PATH)
    find_path(${PREFIX}_INCLUDE_DIRS ${INC_NAME} HINTS ENV CPATH ENV C_INCLUDE_PATH)
endfunction()

function(join OUTPUT SEPARATOR)
    if(ARGC LESS 3)
        set("${OUTPUT}" "")
        return()
    endif()

    list(GET ARGN 0 TEXT)
    list(REMOVE_AT ARGN 0)

    foreach(ITEM IN LISTS ARGN)
        set(TEXT "${TEXT}${SEPARATOR}${ITEM}")
    endforeach()

    set(${OUTPUT} "${TEXT}" PARENT_SCOPE)
endfunction()

function(show_target NAME)
    message("-- Target: ${NAME}")
    get_property(TYPE TARGET ${NAME} PROPERTY TYPE)
    get_property(COMPILE_OPTIONS TARGET ${NAME} PROPERTY COMPILE_OPTIONS)
    get_property(COMPILE_DEFINITIONS TARGET ${NAME} PROPERTY COMPILE_DEFINITIONS)
    get_property(INCLUDE_DIRECTORIES TARGET ${NAME} PROPERTY INCLUDE_DIRECTORIES)
    get_property(LINK_LIBRARIES TARGET ${NAME} PROPERTY LINK_LIBRARIES)

    join(COMPILE_TEXT " " ${COMPILE_OPTIONS} ${COMPILE_DEFINITIONS})
    set(COMPILE_TEXT "${COMPILE_TEXT}${CMAKE_CXX_FLAGS}")
    join(INCLUDE_TEXT " " ${INCLUDE_DIRECTORIES})
    join(LINK_TEXT " " ${LINK_LIBRARIES})
    string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_UPPER)

    message("-- Target type: ${TYPE}")
    message("-- Build type: ${CMAKE_BUILD_TYPE}")
    message("-- Compiler flags: ${COMPILE_TEXT}")
    message("-- Build type flags: ${CMAKE_CXX_FLAGS_${BUILD_UPPER}}")
    message("-- Linker flags: ${CMAKE_CXX_FLAGS}${CMAKE_SHARED_LINKER_FLAGS}")
    message("-- Include directories: ${INCLUDE_TEXT}")
    message("-- Link libraries: ${LINK_TEXT}")
    message("-- Binary data directory: ${CRIO_LINUX_LIBDIR}")
endfunction()
