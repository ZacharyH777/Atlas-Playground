#include "ui_properties_panel.hpp"
#include <core/update_handlers/sync_update.hpp>

namespace atlas {

ui_properties_panel::ui_properties_panel(flecs::world &p_world,
                                         flecs::entity &p_selected_entity)
    : m_selected_entity(p_selected_entity) {
  m_world = p_world;
  atlas::attach(this, &ui_properties_panel::on_ui_update);
}

void ui_properties_panel::on_ui_update() {
  if (ImGui::Begin("Properties Panel")) {
    if (m_selected_entity.is_valid()) {
      m_selected_entity.each([&](flecs::id id) {
        if (id.is_pair()) {
          return;
        }
        // console_log_fatal("component type: {}", (uint64_t)id);
        flecs::entity component_type = id.entity();
        if (!component_type)
          return;

        void *comp_ptr = m_selected_entity.get_mut(id);
        if (!comp_ptr)
          return;

        if (!component_type.has<flecs::Struct>())
          return;

        const std::string label = component_type.name().c_str();

        atlas::ui::draw_panel_component<void>(
            label, [&]() { draw_component(component_type, comp_ptr); });
      });
    }
  }
  ImGui::End();
}

void ui_properties_panel::draw_component(flecs::entity component_entity,
                                         void *component_data) {
  component_entity.each([&](flecs::id id) {
    if (id.is_pair()) {
      return;
    }
    flecs::entity member_entity = id.entity();
    if (member_entity.has<flecs::Member>()) {
      draw_member(member_entity, component_data);
    }
  });
}

void ui_properties_panel::draw_member(const flecs::entity &member_entity,
                                      void *base_ptr) {
  auto *member = member_entity.get<flecs::Member>();
  if (!member) {
    ImGui::Text("%s: <not a member>", member_entity.name().c_str());
    return;
  }

  // Check that offset is explicitly set
  if (!member->use_offset) {
    ImGui::Text("%s: <offset not set>", member_entity.name().c_str());
    return;
  }

  // Ensure base_ptr is not null
  if (!base_ptr) {
    ImGui::Text("%s: <invalid base pointer>", member_entity.name().c_str());
    return;
  }

  console_log_info("Getting Here!");
  void *member_ptr = static_cast<char *>(base_ptr) + member->offset;
  flecs::id_t type_id = member->type;
  console_log_info("Getting Here!2");
  const std::string label = member_entity.name().c_str();
  console_log_info("Getting Here!3");
  console_log_info("World info: {}", m_world.get_info()->name_prefix);

  //  This is a bit rushed and should be a drawer
  //  But is fine for now
  flecs::id_t float_id = m_world.component<float>().id();
  flecs::id_t int_id = m_world.component<int>().id();
  flecs::id_t bool_id = m_world.component<bool>().id();
  flecs::id_t vec3_id = m_world.component<glm::vec3>().id();
  flecs::id_t vec4_id = m_world.component<glm::vec4>().id();
  flecs::id_t highp_vec3_id = m_world.component<glm::highp_vec3>().id();
  flecs::id_t highp_vec4_id = m_world.component<glm::highp_vec4>().id();
  flecs::id_t uint32_t_id = m_world.component<uint32_t>().id();

  console_log_info("Getting Here!4");

  if (type_id == float_id) {
    atlas::ui::draw_float(label, *reinterpret_cast<float *>(member_ptr));
  } else if (type_id == int_id || type_id == uint32_t_id) {
    int &val = *reinterpret_cast<int *>(member_ptr);
    ImGui::InputInt(label.c_str(), &val);
  } else if (type_id == bool_id) {
    bool &val = *reinterpret_cast<bool *>(member_ptr);
    ImGui::Checkbox(label.c_str(), &val);
  } else if (type_id == vec3_id) {
    atlas::ui::draw_vec3(label, *reinterpret_cast<glm::vec3 *>(member_ptr));
  } else if (type_id == highp_vec3_id) {
    atlas::ui::draw_vec3(label,
                         *reinterpret_cast<glm::highp_vec3 *>(member_ptr));
  } else if (type_id == vec4_id) {
    atlas::ui::draw_vec4(label, *reinterpret_cast<glm::vec4 *>(member_ptr));
  } else if (type_id == highp_vec4_id) {
    atlas::ui::draw_vec4(label,
                         *reinterpret_cast<glm::highp_vec4 *>(member_ptr));
  } else {
    console_log_info("Getting here!, no");
    ImGui::Text("%s: <unsupported type id=%llu>", label.c_str(),
                static_cast<unsigned long long>(type_id));
  }
}

} // namespace atlas
