cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(main)

# you should modify THIS
set(LIBS "")
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Linux
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # Windows
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*)

add_executable(main ${SRC_FILES})
target_link_libraries(main ${LIBS})
