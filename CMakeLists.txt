cmake_minimum_required(VERSION 3.22)
project(VoxelEngine VERSION 0.1 LANGUAGES C CXX)

# --- Kompilator ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wconversion)
endif()

# --- Znajdź biblioteki ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# --- Ścieżki do nagłówków ---
include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIRS}
    include/imgui
    include/imgui/backends
    include/glad/include
)

# --- Pliki źródłowe projektu ---
file(GLOB_RECURSE SOURCES 
    "main.cpp"
    "src/*.cpp"
    "src/Game/*.cpp"
    "src/Renderer/*.cpp"
    "src/Shader/*.cpp"
    "src/TextureLoader/*.cpp"
    "src/Camera/*.cpp"
    "src/World/*.cpp"
)

list(FILTER SOURCES EXCLUDE REGEX "include/imgui/examples/.*")

# --- Pliki ImGui ---
set(IMGUI_SOURCES
    include/imgui/imgui.cpp
    include/imgui/imgui_draw.cpp
    include/imgui/imgui_tables.cpp
    include/imgui/imgui_widgets.cpp
    include/imgui/backends/imgui_impl_glfw.cpp
    include/imgui/backends/imgui_impl_opengl3.cpp
    include/glad/src/glad.c
)

# --- Tworzenie wykonywalnego pliku ---
add_executable(VoxelEngine ${SOURCES} ${IMGUI_SOURCES})

# --- Linkowanie bibliotek ---
target_link_libraries(VoxelEngine PRIVATE OpenGL::GL glfw)

# --- Linux specyficzne ---
if(UNIX AND NOT APPLE)
    target_link_libraries(VoxelEngine PRIVATE dl pthread)
endif()

