cmake_minimum_required(VERSION 3.22)
project(MetalCMake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -fno-objc-arc")

include_directories(include/metal-cpp include/metal-cpp-extensions)

add_executable(MetalCMake main.cpp metal.h renderer/Renderer.cpp renderer/Renderer.h)

find_library(FOUNDATION Foundation)
find_library(METAL Metal)
find_library(METAL_KIT MetalKit)
find_library(APP_KIT AppKit)
find_library(COCOA Cocoa)
find_library(CORE_GRAPHICS CoreGraphics)

target_link_libraries(MetalCMake ${FOUNDATION} ${METAL} ${METAL_KIT} ${APP_KIT} ${COCOA} ${CORE_GRAPHICS})
