cmake_minimum_required(VERSION 3.25.1)

project(HPASLT)

# Project settings.
set(CMAKE_CXX_STANDARD 17)

# CMake variables.
set(INCLUDES "")
set(LIBS "")

# SPDLog
add_subdirectory(external/spdlog)
list(APPEND INCLUDES external/spdlog/include)
list(APPEND LIBS spdlog)

# GLEW
add_subdirectory(external/glew)
list(APPEND INCLUDES external/glew/include)
list(APPEND LIBS libglew_static)

# GLFW
add_subdirectory(external/glfw)
list(APPEND INCLUDES external/glfw/include)
list(APPEND LIBS glfw)

# Main executable.
add_executable(hpaslt src/main.cpp)
target_include_directories(hpaslt PUBLIC ${INCLUDES})
target_link_libraries(hpaslt ${LIBS})