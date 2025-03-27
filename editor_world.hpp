#pragma once
#include "level_scene.hpp"

class editor_world {
public:
    editor_world();
    editor_world(const std::string& p_tag);

private:
    atlas::world_scope m_main_world;
    atlas::ref<level_scene> m_first_scene;
};