set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(vroom STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(vroom PUBLIC ${ENGINE_INCLUDE_DIR})

find_package(Threads REQUIRED)

target_link_libraries(vroom
    PUBLIC
        glfw
        Vulkan::Vulkan
        glm::glm
        ImGui
        Threads::Threads
        nlohmann_json::nlohmann_json
)

# Mono embedding support for C# scripting
option(VROOM_ENABLE_MONO "Enable Mono C# scripting support" ON)

if(VROOM_ENABLE_MONO)
    # Skip pkg-config to avoid hangs - directly set paths on macOS
    if(APPLE)
        # Try to find Mono using standard CMake mechanisms
        find_path(MONO_INCLUDE_DIR
            NAMES mono/jit/jit.h
            HINTS
                "/opt/homebrew/include"
                "/usr/local/include"
                "/Library/Frameworks/Mono.framework/Headers"
                "/Library/Frameworks/Mono.framework/Versions/Current/include"
            PATH_SUFFIXES mono-2.0
        )

        find_library(MONO_LIBRARY
            NAMES mono-2.0 mono
            HINTS
                "/opt/homebrew/lib"
                "/usr/local/lib"
                "/Library/Frameworks/Mono.framework/Libraries"
                "/Library/Frameworks/Mono.framework/Versions/Current/lib"
            )

        if(MONO_INCLUDE_DIR AND MONO_LIBRARY)
            # Check for architecture mismatch on Apple Silicon
            if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64" AND MONO_LIBRARY MATCHES "Mono.framework")
                 message(WARNING "Detected Mono Framework on Apple Silicon. This is often x86_64 only. If you experience linker warnings or errors, install Mono via Homebrew (brew install mono) or disable Mono support.")
            endif()

            target_include_directories(vroom PUBLIC ${MONO_INCLUDE_DIR})
            target_link_libraries(vroom PUBLIC ${MONO_LIBRARY})
            target_compile_definitions(vroom PUBLIC VROOM_WITH_MONO=1)
            message(STATUS "Mono found: ${MONO_LIBRARY}")
        else()
            message(WARNING "Mono not found. C# scripting will be disabled.")
        endif()
    endif()
endif()

target_compile_definitions(vroom PUBLIC VROOM_WITH_IMGUI=1)

# Compile shaders to SPIR-V
find_program(GLSL_COMPILER glslc)
if(GLSL_COMPILER)
    set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
    set(SHADER_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/shaders)
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})
    
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/shader.vert.spv
        COMMAND ${GLSL_COMPILER} -fshader-stage=vert -o ${SHADER_OUTPUT_DIR}/shader.vert.spv ${SHADER_DIR}/shader.vert
        DEPENDS ${SHADER_DIR}/shader.vert
        COMMENT "Compiling vertex shader"
    )
    
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_DIR}/shader.frag.spv
        COMMAND ${GLSL_COMPILER} -fshader-stage=frag -o ${SHADER_OUTPUT_DIR}/shader.frag.spv ${SHADER_DIR}/shader.frag
        DEPENDS ${SHADER_DIR}/shader.frag
        COMMENT "Compiling fragment shader"
    )
    
    add_custom_target(shaders DEPENDS ${SHADER_OUTPUT_DIR}/shader.vert.spv ${SHADER_OUTPUT_DIR}/shader.frag.spv)
    add_dependencies(vroom shaders)
    
    # Prepare staging area for packaging
    set(ENGINE_ASSETS_STAGING "${CMAKE_CURRENT_BINARY_DIR}/staging")
    
    add_custom_command(
        OUTPUT ${ENGINE_ASSETS_STAGING}/shaders/shader.vert.spv ${ENGINE_ASSETS_STAGING}/shaders/shader.frag.spv
               ${ENGINE_ASSETS_STAGING}/shaders/shader.vert ${ENGINE_ASSETS_STAGING}/shaders/shader.frag
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ENGINE_ASSETS_STAGING}/shaders
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_OUTPUT_DIR}/shader.vert.spv ${ENGINE_ASSETS_STAGING}/shaders/shader.vert.spv
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_OUTPUT_DIR}/shader.frag.spv ${ENGINE_ASSETS_STAGING}/shaders/shader.frag.spv
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_DIR}/shader.vert ${ENGINE_ASSETS_STAGING}/shaders/shader.vert
        COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_DIR}/shader.frag ${ENGINE_ASSETS_STAGING}/shaders/shader.frag
        DEPENDS ${SHADER_OUTPUT_DIR}/shader.vert.spv ${SHADER_OUTPUT_DIR}/shader.frag.spv
    )

    # Package engine assets
    set(ENGINE_PACKAGE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine/engine.vrpk")
    add_custom_command(
        OUTPUT ${ENGINE_PACKAGE}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/engine
        COMMAND $<TARGET_FILE:vroom_packager> ${ENGINE_ASSETS_STAGING} ${ENGINE_PACKAGE}
        DEPENDS vroom_packager ${ENGINE_ASSETS_STAGING}/shaders/shader.vert.spv ${ENGINE_ASSETS_STAGING}/shaders/shader.frag.spv
        COMMENT "Packaging engine assets..."
    )

    add_custom_target(package_engine_assets ALL DEPENDS ${ENGINE_PACKAGE})
    add_dependencies(vroom package_engine_assets)
else()
    message(WARNING "glslc not found. Shaders will not be compiled. Install Vulkan SDK to compile shaders.")
endif()
