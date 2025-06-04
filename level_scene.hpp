#pragma once
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <physics/physics_3d/physics.hpp>
#include <assets/scripts/camera/editor_camera.hpp>
#include <assets/scripts/camera/main_camera.hpp>
#include <assets/scripts/camera/camera_manager.hpp>
#include <assets/scripts/editor/editor_setup.hpp>
#include <assets/scripts/gui/draw_transforms.hpp>
#include <assets/scripts/gui/ui_hierarchy.hpp>
#include <assets/scripts/gui/ui_properties_panel.hpp>
#include <glm/glm.hpp>

class level_scene : public atlas::scene_scope {
public:
    level_scene();

    level_scene(const std::string& p_tag);

    virtual ~level_scene() {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    void on_physics_update();

    void initialize();
    void start_runtime();
    void stop_runtime();

private:
    // Scene Objects
    atlas::ref<atlas::physics::physics_engine> engine;
    atlas::ref<atlas::scene_object> m_sphere;
    atlas::ref<atlas::scene_object> m_platform;
    atlas::ref<atlas::scene_object> m_test_object;
    atlas::ref<atlas::scene_object> m_test_object2;
    atlas::ref<atlas::scene_object> m_test_object3;

    // Spine
    atlas::ref<atlas::scene_object> m_player_head;
    atlas::ref<atlas::scene_object> m_player_torso;
    atlas::ref<atlas::scene_object> m_player_pelvis;

    // Left side
    atlas::ref<atlas::scene_object> m_player_larm;
    atlas::ref<atlas::scene_object> m_player_lforearm;
    atlas::ref<atlas::scene_object> m_player_lhand;
    atlas::ref<atlas::scene_object> m_player_lthigh;
    atlas::ref<atlas::scene_object> m_player_lshin;
    atlas::ref<atlas::scene_object> m_player_lfoot;

    // Right side
    atlas::ref<atlas::scene_object> m_player_rarm;
    atlas::ref<atlas::scene_object> m_player_rforearm;
    atlas::ref<atlas::scene_object> m_player_rhand;
    atlas::ref<atlas::scene_object> m_player_rthigh;
    atlas::ref<atlas::scene_object> m_player_rshin;
    atlas::ref<atlas::scene_object> m_player_rfoot;

    bool m_is_simulation_enabled = false;
    atlas::ref<atlas::scene_object> m_main_camera;
    atlas::ref<atlas::scene_object> m_camera;

    bool test_bool = false;
    bool test_bool_2 = false;

    //Actions
    atlas::ref<editor_camera> m_editor_camera;
    atlas::ref<main_camera> m_main_controls;
    atlas::ref<camera_manager> m_camera_manager;
    atlas::ref<editor_setup> m_editor_setup;
    atlas::ref<draw_transforms> m_draw_transforms;
    atlas::ref<ui_hierarchy> m_ui_hierarchy;
    atlas::ref<atlas::ui_properties_panel> m_ui_properties;

    flecs::entity test = flecs::entity::null();

    // Determine object vs component
    flecs::entity object;
    flecs::entity is;
    flecs::entity object_test;
};