#pragma once
#include <core/scene/scene.hpp>
#include <core/scene/scene_object.hpp>
#include <glm/glm.hpp>

class level_scene : public atlas::scene_scope {
public:
    level_scene();

    level_scene(const std::string& p_tag);

    virtual ~level_scene() {
        console_log_error("LevelScene::~LevelScene Destructed!!!");
    }

    void on_update();

    void on_ui_update();

    void on_physics_update();

    void on_runtime_start();
    void on_runtime_stop();

private:
    atlas::ref<atlas::scene_object> m_sphere;
    atlas::ref<atlas::scene_object> m_platform;
    atlas::ref<atlas::scene_object> m_light_object;
    bool m_is_simulation_enabled = false;
    atlas::ref<atlas::scene_object> m_camera;

    glm::vec2 last_cursor_pos = glm::zero<glm::vec2>();
};