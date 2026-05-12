cmake_minimum_required(VERSION 3.20)

# 1. Project Definition
project(tecs CXX)

# The fix for Raylib's ancient CMake requirement
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# Enforce modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- THE FIX FOR SPDLOG WARNINGS ---
# Silence the massive wall of MSVC deprecation warnings from spdlog
add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

include(FetchContent)

# ---------------------------------------------------------
# 2. Fetch spdlog (Logging)
# ---------------------------------------------------------
FetchContent_Declare(spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git 
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# ---------------------------------------------------------
# 3. Fetch Raylib
# ---------------------------------------------------------
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib 
    GIT_REPOSITORY https://github.com/raysan5/raylib.git 
    GIT_TAG master   # <-- CHANGED TO MASTER
)
FetchContent_MakeAvailable(raylib)

# ---------------------------------------------------------
# 4. Fetch Dear ImGui & rlImGui (Raylib Backend)
# ---------------------------------------------------------
FetchContent_Declare(imgui 
    GIT_REPOSITORY https://github.com/ocornut/imgui.git 
    GIT_TAG docking 
)
FetchContent_Populate(imgui)

FetchContent_Declare(rlimgui 
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git 
    GIT_TAG main
)
FetchContent_Populate(rlimgui)

# Manually create the ImGui static library target
add_library(ImGui STATIC 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)
target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR} ${rlimgui_SOURCE_DIR})
target_link_libraries(ImGui PUBLIC raylib)

# ---------------------------------------------------------
# 5. Define the Engine Library
# ---------------------------------------------------------
add_library(Engine STATIC 
    Engine/src/ECS.cpp
)
target_include_directories(Engine PUBLIC Engine/include)

# Link our dependencies into the Engine
target_link_libraries(Engine PUBLIC spdlog::spdlog raylib ImGui)

# ---------------------------------------------------------
# 6. Define the Sandbox Executable
# ---------------------------------------------------------
add_executable(Sandbox 
    Sandbox/main.cpp
)

# Link the Engine to the game
target_link_libraries(Sandbox PRIVATE Engine)