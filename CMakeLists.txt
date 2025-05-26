cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)

project(SSAK VERSION 0.1.0)

find_package(LibArchive REQUIRED)
find_package(SQLite3 REQUIRED)

add_subdirectory(src/tools)
add_subdirectory(test)
