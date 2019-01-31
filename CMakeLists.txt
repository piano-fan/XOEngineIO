cmake_minimum_required(VERSION 3.8)
project(XO)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp output.h output.cpp input.h input.cpp timer.h)
include_directories(XOEngineCore/Include)

add_executable(XO ${SOURCE_FILES})
add_subdirectory(XOEngineCore)

set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static-libstdc++ -static")

target_link_libraries(XO XO_CORE)
