cmake_minimum_required(VERSION 3.27)
project(Starter CXX)

set(SCRIPTS assets/scripts)

build_application(
    SOURCES
    application.cpp
    
    editor_world.hpp
    editor_world.cpp
    assets/data/data.hpp
    assets/data/my_controls_config.hpp
    ${SCRIPTS}/keepy.cpp
    ${SCRIPTS}/keepy.hpp

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