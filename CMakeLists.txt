cmake_minimum_required(VERSION 3.25.1)

project(HPASLT)

# --------------------- Project settings --------------------- #

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# ---------------------- CMake variables --------------------- #

set(LIBS "")

# -------------------- External Libraries -------------------- #

# SPDLog
add_subdirectory(external/spdlog)

# Eventpp
add_subdirectory(external/eventpp)

# Audio File
add_subdirectory(external/audiofile)

# Port Audio
add_subdirectory(external/portaudio)

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

# ----------------------- ImGui Console ---------------------- #

set(IMGUI_CONSOLE_DIR external/imguiconsole)

# CSYS
add_library(csys INTERFACE)
target_include_directories(csys INTERFACE ${IMGUI_CONSOLE_DIR}/include)
target_compile_features(csys INTERFACE cxx_std_17)

# ImGui Console
file(
    GLOB
    IMGUI_CONSOLE_SRC
    ${IMGUI_CONSOLE_DIR}/src/imgui_console.cpp
    ${IMGUI_CONSOLE_DIR}/include/imgui_console/imgui_console.h
)
add_library(imgui_console STATIC ${IMGUI_CONSOLE_SRC})
target_include_directories(
    imgui_console
    PUBLIC
    external/imgui
    ${IMGUI_CONSOLE_DIR}/include/imgui_console
)
target_compile_features(imgui_console PUBLIC cxx_std_17)
target_link_libraries(imgui_console PUBLIC csys)

# ------------------------------------------------------------ #
#                            HPASLT                            #
# ------------------------------------------------------------ #

# ---------------------- Infrastructure ---------------------- #

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

# Commands.
file(
    GLOB_RECURSE
    HPASLT_COMMANDS_SRC
    src/commands/*.cpp
    src/commands/*.h
)
add_library(hpaslt_commands ${HPASLT_COMMANDS_SRC})
target_include_directories(
    hpaslt_commands
    PUBLIC
    external/eventpp/include
    external/csys/include
    src
)
target_link_libraries(
    hpaslt_commands
    hpaslt_logger
    eventpp
)
list(APPEND LIBS hpaslt_commands)

# Serialization.
file(
    GLOB_RECURSE
    HPASLT_SERIALIZATION_SRC
    src/serialization/*.cpp
    src/serialization/*.h
)
add_library(hpaslt_serialization ${HPASLT_SERIALIZATION_SRC})
target_include_directories(
    hpaslt_serialization
    PUBLIC
    external/spdlog/include
    external/eventpp/include
    external/cereal/include
    src
)
target_link_libraries(
    hpaslt_serialization
    hpaslt_logger
    spdlog
    eventpp
)
list(APPEND LIBS hpaslt_serialization)

# --------------------------- Core --------------------------- #

file(
    GLOB_RECURSE
    HPASLT_CORE_SRC
    src/core/*.cpp
    src/core/*.h
)
add_library(hpaslt_core ${HPASLT_CORE_SRC})
target_include_directories(
    hpaslt_core
    PUBLIC
    src
)
target_link_libraries(
    hpaslt_core
    hpaslt_commands
)
list(APPEND LIBS hpaslt_core)

# ------------------------- Rendering ------------------------ #

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
    external/imguiconsole/include
    src
)
target_link_libraries(
    hpaslt_frontend
    hpaslt_commands
    hpaslt_window_mgr
    hpaslt_logger
    hpaslt_serialization
    spdlog
    eventpp
    imgui
    imgui_console
)
list(APPEND LIBS hpaslt_frontend)

# ------------------------------------------------------------ #
#                        Main executable                       #
# ------------------------------------------------------------ #

add_executable(hpaslt src/main.cpp)
target_include_directories(hpaslt PUBLIC src)
target_link_libraries(hpaslt ${LIBS})

# ------------------------------------------------------------ #
#                           Resources                          #
# ------------------------------------------------------------ #

# Fonts
add_custom_target(
    hpaslt_fonts ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/fonts
    ${PROJECT_BINARY_DIR}/fonts
    COMMENT "Copying font resources."
)
add_dependencies(hpaslt hpaslt_fonts)