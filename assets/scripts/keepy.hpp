#pragma once
#include <core/scene/scene.hpp>

class keepy  {
public:
    keepy();
    ~keepy();

    void on_update();
    void on_physics_update();
    void on_ui_update();
private:
    atlas::ref<atlas::scene_scope> m_current_scene=nullptr;
};

