cmake_minimum_required(VERSION 3.27)
project(Systemy_Operacyjne_2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Systemy_Operacyjne_2 main.cpp)

target_link_libraries(Systemy_Operacyjne_2 pthread)