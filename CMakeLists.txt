cmake_minimum_required(VERSION 3.27)
project(Starter CXX)

set(SCRIPTS assets/scripts)
set(TEST testing_purposes_only)
set(DATA assets/data)

build_application(
    SOURCES
    application.cpp
    
    editor_world.hpp
    editor_world.cpp
    ${DATA}/camera/editor_controls.hpp
    ${DATA}/camera/camera_data.hpp
    ${DATA}/editor/editor_data.hpp

    ${TEST}/requirement_handling/requirements.hpp
    ${TEST}/requirement_handling/requirements.cpp
    ${TEST}/requirement_handling/component_builder.hpp
    ${TEST}/requirement_handling/offset_of.hpp

    ${SCRIPTS}/camera/editor_camera.cpp
    ${SCRIPTS}/camera/editor_camera.hpp

    ${SCRIPTS}/camera/main_camera.cpp
    ${SCRIPTS}/camera/main_camera.hpp

    ${SCRIPTS}/camera/camera_manager.cpp
    ${SCRIPTS}/camera/camera_manager.hpp

    ${SCRIPTS}/editor/editor_setup.cpp
    ${SCRIPTS}/editor/editor_setup.hpp

    ${SCRIPTS}/gui/draw_transforms.cpp
    ${SCRIPTS}/gui/draw_transforms.hpp

    ${SCRIPTS}/gui/ui_hierarchy.cpp
    ${SCRIPTS}/gui/ui_hierarchy.hpp

    ${SCRIPTS}/gui/ui_properties_panel.hpp
    ${SCRIPTS}/gui/ui_properties_panel.cpp



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