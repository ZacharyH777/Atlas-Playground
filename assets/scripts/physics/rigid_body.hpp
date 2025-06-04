#pragma once
#include <assets/data/camera/editor_controls.hpp>
#include <assets/data/editor/editor_data.hpp>
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>


class rigid_body {
public:
  rigid_body() = delete;
  rigid_body(flecs::world &p_registery);
  void attach_entity(atlas::ref<atlas::scene_object> p_entity,
                     flecs::world &p_registery);
  ~rigid_body();

  void on_update();

private:
  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;
  struct rigid_body_action {};

  // Scene should not be stored here but we have no way to query all scenes:
  // So for now I am store level_scene here

  flecs::world m_registery;
  flecs::query<atlas::transform, atlas::camera, editor_controls, editor_data>
      m_query;
};
