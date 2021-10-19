cmake_minimum_required(VERSION 3.21.2)

# Show error if build_dir is not equal to build/
if(NOT ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}/build)
    message(FATAL_ERROR "Please set your build dir to build. cmake -B build/")
endif()

project(RayTracingInWeekend)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*.h src/*cpp)
add_executable(main ${SOURCE_FILES})

target_include_directories(main
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
