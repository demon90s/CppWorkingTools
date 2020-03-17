cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(main)

function(group_by_dir src_dir)
    if(MSVC)
        foreach(FILE ${ARGN})
            get_filename_component(FULL_NAME "${FILE}" ABSOLUTE)
            get_filename_component(PARENT_DIR "${FULL_NAME}" PATH)
            string(REPLACE "${ARGV0}" "" GROUP "${PARENT_DIR}")
            string(REPLACE "/" "\\" GROUP "${GROUP}")
            source_group("${GROUP}" FILES "${FILE}")
        endforeach()
    endif(MSVC)
endfunction(group_by_dir)

# you should modify THIS
set(LIBS "")
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux
	set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
	set(LIBS gtest pthread)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows
    set(LIBS gtestd.lib)
    add_definitions(/MTd)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*)
group_by_dir(${PROJECT_SOURCE_DIR} ${SRC_FILES})

add_executable(main ${SRC_FILES})
target_link_libraries(main ${LIBS})
