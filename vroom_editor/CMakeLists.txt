file(GLOB_RECURSE VROOM_EDITOR_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE VROOM_EDITOR_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_executable(vroom_editor ${VROOM_EDITOR_SOURCES} ${VROOM_EDITOR_HEADERS})

target_link_libraries(vroom_editor PRIVATE vroom ImGui)

target_compile_definitions(vroom_editor PRIVATE VROOM_WITH_IMGUI=1)

# Copy everything in the assets directory to the vroom_editor build directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    add_custom_command(TARGET vroom_editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        COMMENT "Copying assets directory"
    )
endif()