if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ElixirTests)
    include(${CMAKE_SOURCE_DIR}/cmake/settings.cmake)
endif()

add_subdirectory(loops)
