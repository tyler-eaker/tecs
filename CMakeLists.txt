cmake_minimum_required(VERSION 3.20)

project(tecs CXX)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)

include(FetchContent)

FetchContent_Declare(spdlog 
    GIT_REPOSITORY https://github.com/gabime/spdlog.git 
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(raylib 
    GIT_REPOSITORY https://github.com/raysan5/raylib.git 
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

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

add_library(ImGui STATIC 
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)
target_include_directories(ImGui PUBLIC ${imgui_SOURCE_DIR} ${rlimgui_SOURCE_DIR})
target_link_libraries(ImGui PUBLIC raylib)

add_library(Engine STATIC 
    Engine/src/ECS.cpp
)
target_include_directories(Engine PUBLIC Engine/include)

target_link_libraries(Engine PUBLIC spdlog::spdlog raylib ImGui)

add_executable(Sandbox 
    Sandbox/main.cpp
)

target_link_libraries(Sandbox PRIVATE Engine)