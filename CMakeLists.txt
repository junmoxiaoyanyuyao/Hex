cmake_minimum_required(VERSION 3.5.0)
project(HEX)


aux_source_directory(./source SourceFile)
add_executable(HEX ${SourceFile})

set(CMAKE_BUILD_TYPE "Debug")
