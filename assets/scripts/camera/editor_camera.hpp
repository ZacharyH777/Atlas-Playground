#pragma once
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>
#include <assets/data/camera/editor_controls.hpp>

class editor_camera {
public:
  editor_camera() = delete;
  editor_camera(flecs::world& p_registery);
  void attach_entity(atlas::ref<atlas::scene_object> p_entity,
                     flecs::world& p_registery);
  ~editor_camera();

  void on_update();

private:
  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;
  struct editor_camera_action{};

  // Scene should not be stored here but we have no way to query all scenes:
  // So for now I am store level_scene here

  flecs::world m_registery;
  flecs::query<atlas::transform, atlas::camera, editor_controls> m_query;
};
