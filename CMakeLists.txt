cmake_minimum_required(VERSION 3.21.0)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(Elixir LANGUAGES C CXX VERSION 0.1.0)
    include(${CMAKE_SOURCE_DIR}/cmake/settings.cmake)
endif()

add_subdirectory(src)

option(ELIXIR_BUILD_TESTS "Build unit tests for Elixir" OFF)

if(ELIXIR_BUILD_TESTS)
    add_subdirectory(tests)
endif()