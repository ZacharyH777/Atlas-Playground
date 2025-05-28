#pragma once
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>
#include <assets/data/camera/camera_data.hpp>
#include <assets/data/editor/editor_data.hpp>

class editor_setup {
public:
  editor_setup() = delete;
  editor_setup(flecs::world& p_registery);

  ~editor_setup();

  void on_update();

  bool runtime_on = false;

private:
  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;
  struct editor_setup_action{};

  // Scene should not be stored here but we have no way to query all scenes:
  // So for now I am store level_scene here

  flecs::world m_registery;
  flecs::query<editor_data> m_query;
};
