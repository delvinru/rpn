cmake_minimum_required(VERSION 3.15)
project(clion_polka C)

set(CMAKE_C_STANDARD 11)
#add_executable(clion_polka main.c)
add_executable(clion_polka main.c)
target_link_libraries(clion_polka m)
