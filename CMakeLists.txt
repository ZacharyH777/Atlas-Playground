cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    editor_world.hpp
    editor_world.cpp

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

generate_compile_commands()