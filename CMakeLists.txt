cmake_minimum_required(VERSION 3.16...4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(libjunk VERSION 1.0
  DESCRIPTION "General purpose library"
  LANGUAGES C)
add_library(junk SHARED)
add_subdirectory(src)
target_include_directories(junk PUBLIC include)
