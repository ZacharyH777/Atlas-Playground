#pragma once
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>
#include <assets/data/camera/camera_data.hpp>
#include <assets/data/editor/editor_data.hpp>

class camera_manager {
public:
  camera_manager() = delete;
  camera_manager(flecs::world& p_registery);
  void attach_entity(atlas::ref<atlas::scene_object> p_entity,
                     flecs::world& p_registery);
  ~camera_manager();

  void on_update();

private:
  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;
  struct camera_manager_action{};

  // Scene should not be stored here but we have no way to query all scenes:
  // So for now I am store level_scene here

  flecs::world m_registery;
  flecs::query<atlas::camera, camera_data, editor_data> m_query;
};
