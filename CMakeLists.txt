cmake_minimum_required(VERSION 3.14)

set(INCLUDE_DIR src/include)
set(CMAKE_EXPORT_COMPILE_COMANDS on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-O3")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-O5")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/O2")
endif()

project(tyrotical CXX)

add_executable(
    tyrotical
    src/Ayanami.cpp
    src/device.cpp
    src/window.cpp
    src/pipeline.cpp
    src/tyrotical.cpp
)

target_include_directories(tyrotical PRIVATE ${INCLUDE_DIR} vendor/include ${VULKAN_PATH}/Include)
target_link_directories(tyrotical PRIVATE vendor/lib ${VULKAN_PATH}/Lib)
target_link_libraries(tyrotical PRIVATE glfw3 vulkan-1)