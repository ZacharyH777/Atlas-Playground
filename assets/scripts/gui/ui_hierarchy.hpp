#pragma once
#include <core/scene/scene.hpp>
#include <testing_purposes_only/requirement_handling/requirements.hpp>
#include <assets/data/camera/camera_data.hpp>
#include <assets/data/editor/editor_data.hpp>
#include <imgui.h>

class ui_hierarchy {
public:
  ui_hierarchy() = delete;
  ui_hierarchy(flecs::world& p_registery, flecs::entity& p_selected_entity);

  ~ui_hierarchy();

  void on_ui_update();

private:
  void draw_entity_tree(flecs::entity p_entity);

  atlas::ref<atlas::scene_scope> m_current_scene = nullptr;
  atlas::requirements m_requirements;

  flecs::entity& m_selected_entity;
  flecs::world m_registery;
  flecs::query<> m_root_query;
  std::unordered_map<flecs::entity_t, ImVec2> m_entity_positions;
};