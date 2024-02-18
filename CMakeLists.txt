cmake_minimum_required(VERSION 3.26)
project(UHUB)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LuaJIT REQUIRED IMPORTED_TARGET luajit)

add_executable(UHUB
        src/main.cpp
        src/func.cpp
        src/func.h
        src/safe_queue.h
        src/define.h)
target_link_libraries(UHUB PkgConfig::LuaJIT)

