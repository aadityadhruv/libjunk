cmake_minimum_required(VERSION 3.17...4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(libjunk VERSION 1.0
  DESCRIPTION "General purpose library"
  LANGUAGES C)
add_library(junk SHARED)
add_subdirectory(src)
target_include_directories(junk PUBLIC include)
install(TARGETS junk)
install(DIRECTORY include/ DESTINATION include)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()


