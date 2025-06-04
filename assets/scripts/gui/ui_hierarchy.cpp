#include "ui_hierarchy.hpp"
#include <core/update_handlers/sync_update.hpp>
#include <fmt/core.h>

ui_hierarchy::ui_hierarchy(flecs::world &p_registery,
                           flecs::entity &p_selected_entity)
    : m_registery(p_registery), m_selected_entity(p_selected_entity) {

  flecs::entity is = p_registery.lookup("is");
  if (!is.is_valid())
    is = p_registery.entity("is");

  flecs::entity object = p_registery.lookup("object");
  if (!object.is_valid())
    object = p_registery.entity("object");

  m_root_query = p_registery.query_builder()
                     .with(flecs::ChildOf)
                     .second(flecs::Wildcard)
                     .oper(flecs::Not)
                     .with(is, object)
                     .build();

  atlas::attach(this, &ui_hierarchy::on_ui_update);
}

ui_hierarchy::~ui_hierarchy() {}

void ui_hierarchy::on_ui_update() {
  m_entity_positions.clear();

  if (ImGui::Begin("Hierarchy")) {
    m_root_query.each([&](flecs::entity e) { draw_entity_tree(e); });
  }

  ImGui::End();
}

void ui_hierarchy::draw_entity_tree(flecs::entity p_entity) {
  const char *name = p_entity.name();
  std::string label =
      (name[0] == '\0') ? fmt::format("Entity {}", p_entity.id()) : name;

  bool has_children = false;
  p_entity.children([&](flecs::entity) { has_children = true; });

  ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_SpanAvailWidth |
      (m_selected_entity == p_entity ? ImGuiTreeNodeFlags_Selected : 0);

  if (!has_children) {
    flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
  }

  // Draw the tree node
  bool opened = ImGui::TreeNodeEx((void *)(uintptr_t)p_entity.id(), flags,
                                  "%s", label.c_str());

  // Record the center-left position of this tree item
  ImVec2 min = ImGui::GetItemRectMin();
  ImVec2 max = ImGui::GetItemRectMax();
  ImVec2 center_left = ImVec2(min.x, (min.y + max.y) * 0.5f);
  m_entity_positions[p_entity.id()] = center_left;

  // Draw line to parent if one exists and is tracked
  flecs::entity parent = p_entity.parent();
  if (parent.is_valid()) {
    auto it = m_entity_positions.find(parent.id());
    if (it != m_entity_positions.end()) {
      ImVec2 parent_pos = it->second;
      float horizontal_offset = 10.0f;
      float parent_vertical = 5.0f;

      parent_pos.x += horizontal_offset;
      parent_pos.y += parent_vertical;

      float vertical_offset = center_left.y - parent_pos.y;

      ImVec2 vertical = ImVec2(parent_pos.x, parent_pos.y + vertical_offset);
      ImVec2 horizontal = ImVec2(center_left.x, parent_pos.y + vertical_offset);

      ImGui::GetWindowDrawList()->AddLine(parent_pos, vertical,
                                          IM_COL32(150, 150, 150, 255), 1.0f);
      ImGui::GetWindowDrawList()->AddLine(vertical, horizontal,
                                          IM_COL32(150, 150, 150, 155), 2.0f);
    }
  }

  if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
    m_selected_entity = p_entity;
  }

  if (ImGui::BeginPopupContextItem()) {
    if (ImGui::MenuItem("Delete")) {
      p_entity.destruct();
    }
    ImGui::EndPopup();
  }

  if (has_children && opened) {
    p_entity.children([&](flecs::entity child) { draw_entity_tree(child); });
    ImGui::TreePop();
  }
}