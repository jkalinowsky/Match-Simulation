cmake_minimum_required(VERSION 3.26)
project(OpenFM)

set(CMAKE_CXX_STANDARD 17)

add_executable(OpenFM main.cpp
        Player.cpp
        Player.h
        Team.cpp
        Team.h
        Tactic.cpp
        Tactic.h
        Match.cpp
        Match.h)
