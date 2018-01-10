cmake_minimum_required(VERSION 3.9)
project(ACO_TMS)

set(CMAKE_CXX_STANDARD 11)
add_executable(ACO_TMS main.cpp Ant.cpp Ant.h AntColony.cpp AntColony.h Evaluator.cpp Evaluator.h)
