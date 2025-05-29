#pragma once
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>
#include <assets/data/camera/camera_data.hpp>
#include <assets/data/editor/editor_data.hpp>

class draw_transforms {
public:
  draw_transforms() = delete;
  draw_transforms(flecs::world& p_registery);

  ~draw_transforms();

  void on_ui_update();

private:
  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;
  struct draw_transforms_action{};

  // Scene should not be stored here but we have no way to query all scenes:
  // So for now I am store level_scene here

  flecs::world m_registery;
  flecs::query<atlas::transform> m_query;
};
