project (INVADER CXX)
cmake_minimum_required (VERSION 3.4)

set (INVADER_VERSION_MAJOR 0)
set (INVADER_VERSION_MINOR 1)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


set(CMAKE_VERBOSE_MAKEFILE on)

set(WFLAGS -Wall)

#INCLUDES=-I$(ALLEGRO_PATH)/include

#set(CMAKE_CXX_FLAGS "${WFLAGS} -std=c++11 -Wall -g")

add_compile_options(${WFLAGS} -std=c++11 -g -O2)
#add_definitions(-DDEBUG)

add_subdirectory(src)


#configure_file (
#"invader_config.h.in"
#  "include/config.h"
#)


function(add_copy_commands src dest destfilesvar)
    set(destfiles)
    foreach(basename ${ARGN})
        list(APPEND destfiles "${dest}/${basename}")
        add_custom_command(
            OUTPUT  "${dest}/${basename}"
            DEPENDS "${src}/${basename}"
            COMMAND "${CMAKE_COMMAND}" -E copy
                    "${src}/${basename}" "${dest}/${basename}"
            )
    endforeach()
    set(${destfilesvar} "${destfiles}" PARENT_SCOPE)
endfunction()

function(copy_resources_dir_to_build target src dest)
    if(src STREQUAL dest)
        return()
    endif()

    file(GLOB_RECURSE files RELATIVE "${src}" "${src}/*")
    add_copy_commands("${src}" "${dest}" destfiles "${files}")
    add_custom_target(${target} ALL DEPENDS ${destfiles})
endfunction(copy_resources_dir_to_build)

copy_resources_dir_to_build(copy_resource_data
    "${CMAKE_CURRENT_SOURCE_DIR}/resources"
    "${CMAKE_CURRENT_BINARY_DIR}/resources"
    ) 

message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
