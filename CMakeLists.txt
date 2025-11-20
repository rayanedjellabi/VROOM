cmake_minimum_required(VERSION 3.24)
project(VROOM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# GLM still declares compatibility with very old CMake releases; setting this variable
# keeps its bundled CMakeLists from tripping the fatal compatibility check on 3.24+.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

option(VROOM_BUILD_VROOM_EDITOR "Build the VROOM Editor" ON)
option(VROOM_ENABLE_COVERAGE "Enable code coverage" OFF)

# ---- Version Handling ----
# Version can be set via VROOM_VERSION CMake variable (e.g., from GitHub Actions)
# If not set, try to get it from git tag, otherwise default to 0.0.0
if(DEFINED VROOM_VERSION)
    set(VROOM_VERSION_STRING ${VROOM_VERSION})
else()
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_TAG
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
        if(GIT_TAG)
            # Remove 'v' prefix if present
            string(REGEX REPLACE "^v" "" VROOM_VERSION_STRING "${GIT_TAG}")
        else()
            set(VROOM_VERSION_STRING "0.0.0")
        endif()
    else()
        set(VROOM_VERSION_STRING "0.0.0")
    endif()
endif()

# Parse version string into components
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${VROOM_VERSION_STRING}")
if(VERSION_MATCH)
    set(VROOM_VERSION_MAJOR ${CMAKE_MATCH_1})
    set(VROOM_VERSION_MINOR ${CMAKE_MATCH_2})
    set(VROOM_VERSION_PATCH ${CMAKE_MATCH_3})
else()
    # Fallback if version string doesn't match expected format
    set(VROOM_VERSION_MAJOR 0)
    set(VROOM_VERSION_MINOR 0)
    set(VROOM_VERSION_PATCH 0)
endif()

# Get git hash for build info
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE VROOM_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
else()
    set(VROOM_GIT_HASH "unknown")
endif()

message(STATUS "VROOM Version: ${VROOM_VERSION_STRING} (${VROOM_VERSION_MAJOR}.${VROOM_VERSION_MINOR}.${VROOM_VERSION_PATCH})")
message(STATUS "Git Hash: ${VROOM_GIT_HASH}")

# Generate version header
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/include/vroom/core)
configure_file(
    ${PROJECT_SOURCE_DIR}/engine/include/vroom/core/Version.hpp.in
    ${PROJECT_BINARY_DIR}/include/vroom/core/Version.hpp
    @ONLY
)

# Add generated include directory
include_directories(${PROJECT_BINARY_DIR}/include)

include(FetchContent)

# ---- Dependencies ----
find_package(Vulkan REQUIRED)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# Google Test for unit testing
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(NOT TARGET ImGui)
    add_library(ImGui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    )
    target_include_directories(ImGui PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
    )
    target_link_libraries(ImGui PUBLIC glfw Vulkan::Vulkan)
endif()

add_subdirectory(engine)

if(VROOM_BUILD_VROOM_EDITOR)
    add_subdirectory(vroom_editor)
endif()

# Enable testing
enable_testing()

# Add tests subdirectory
add_subdirectory(tests)
