cmake_minimum_required(VERSION 4.0)
project(Prisoner_s_dilemma)

set(CMAKE_CXX_STANDARD 20)

add_executable(Prisoner_s_dilemma main.cpp
        Common.h
        Input_data_processing.h
        Input_data_processing.cpp)
