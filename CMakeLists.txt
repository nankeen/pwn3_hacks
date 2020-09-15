cmake_minimum_required(VERSION 3.10)

project(Pwn3Hack)

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/cheat.cc
    ${PROJECT_SOURCE_DIR}/src/flying_cheat.cc
    ${PROJECT_SOURCE_DIR}/src/speed_cheat.cc
    ${PROJECT_SOURCE_DIR}/src/commands.cc
)

set(CMAKE_CXX_STANDARD 17)

add_library(Cheats SHARED
    ${SOURCES}
)

find_package(X11 REQUIRED)
target_link_libraries(Cheats ${X11_LIBRARIES})
target_include_directories(Cheats PRIVATE ${X11_INCLUDE_DIR})

find_package(X11 REQUIRED)
target_link_libraries(Cheats ${X11_LIBRARIES})
target_include_directories(Cheats PRIVATE ${X11_INCLUDE_DIR})

target_include_directories(Cheats PRIVATE "include")

target_link_libraries(Cheats dl)
