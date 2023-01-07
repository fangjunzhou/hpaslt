cmake_minimum_required(VERSION 3.25.1)

project(HPASLT)

# ----------------------------- Project settings ----------------------------- #

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# ------------------------------ CMake variables ----------------------------- #

set(LIBS "")

# ---------------------------- External Libraries ---------------------------- #

# SPDLog
add_subdirectory(external/spdlog)

# eventpp
add_subdirectory(external/eventpp)

# GLEW
add_subdirectory(external/glew)

# GLFW
add_subdirectory(external/glfw)

# ImGui
file(
    GLOB
    IMGUI_SRC
    external/imgui/*.cpp
    external/imgui/*.h
    external/imgui/backends/imgui_impl_glfw.h
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.h
    external/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui ${IMGUI_SRC})
target_include_directories(
    imgui
    PUBLIC
    external/imgui
    external/imgui/backends
    external/glew/include
    external/glfw/include
)
target_link_libraries(
    imgui
    libglew_static
    glfw
)

# ---------------------------------- HPASLT ---------------------------------- #

# Logger.
file(
    GLOB_RECURSE
    HPASLT_LOGGER_SRC
    src/logger/*.cpp
    src/logger/*.h
)
add_library(hpaslt_logger ${HPASLT_LOGGER_SRC})
target_include_directories(hpaslt_logger PUBLIC external/spdlog/include)
target_link_libraries(hpaslt_logger spdlog)
list(APPEND LIBS hpaslt_logger)

# Window Manager.
file(
    GLOB_RECURSE
    HPASLT_WINDOW_MGR_SRC
    src/window_manager/*.cpp
    src/window_manager/*.h
)
add_library(hpaslt_window_mgr ${HPASLT_WINDOW_MGR_SRC})
target_include_directories(
    hpaslt_window_mgr
    PUBLIC
    external/spdlog/include
    external/eventpp/include
    external/glew/include
    external/glfw/include
    external/imgui
    external/imgui/backends
    src
)
target_link_libraries(
    hpaslt_window_mgr
    hpaslt_logger
    spdlog
    eventpp
    libglew_static
    glfw
    imgui
)
list(APPEND LIBS hpaslt_window_mgr)

# Frontend.
file(
    GLOB_RECURSE
    HPASLT_FRONTEND_SRC
    src/frontend/*.cpp
    src/frontend/*.h
)
add_library(hpaslt_frontend ${HPASLT_FRONTEND_SRC})
target_include_directories(
    hpaslt_frontend
    PUBLIC
    external/spdlog/include
    external/eventpp/include
    external/imgui
    external/imgui/backends
    src
)
target_link_libraries(
    hpaslt_frontend
    hpaslt_window_mgr
    hpaslt_logger
    spdlog
    eventpp
    imgui
)
list(APPEND LIBS hpaslt_frontend)

# Main executable.
add_executable(hpaslt src/main.cpp)
target_include_directories(hpaslt PUBLIC src)
target_link_libraries(hpaslt ${LIBS})