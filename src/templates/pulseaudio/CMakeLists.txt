cmake_minimum_required(VERSION 3.0)

project(pulse_example VERSION 0.1.0)

find_package(PulseAudio REQUIRED)

add_executable(pulse_example pulse_example.c)
target_link_libraries(pulse_example pulse)