cmake_minimum_required(VERSION 3.22)

project(SSAK VERSION 0.1.0)

find_package(LibArchive)
find_package(SQLite3)

add_subdirectory(src)
