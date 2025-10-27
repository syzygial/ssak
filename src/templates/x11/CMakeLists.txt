cmake_minimum_required(VERSION 3.0)

project(x11_example VERSION 0.1.0)

find_package(X11 REQUIRED)

add_executable(x11_example x11_example.c)
target_link_libraries(x11_example X11)