cmake_minimum_required(VERSION 3.27)
project(Starter CXX)

set(SCRIPTS assets/scripts)
set(TEST testing_purposes_only)

build_application(
    SOURCES
    application.cpp
    
    editor_world.hpp
    editor_world.cpp
    assets/data/camera/editor_controls.hpp

    ${TEST}/requirement_handling/requirements.hpp
    ${TEST}/requirement_handling/requirements.cpp

    ${SCRIPTS}/camera/editor_camera.cpp
    ${SCRIPTS}/camera/editor_camera.hpp

    level_scene.hpp
    level_scene.cpp

    PACKAGES
    imguidocking
    stb
    flecs
    tinyobjloader
    atlas

    LINK_PACKAGES

    LINK_PACKAGES
    imguidocking::imguidocking
    stb::stb
    flecs::flecs_static
    tinyobjloader::tinyobjloader


    atlas::atlas
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

generate_compile_commands()