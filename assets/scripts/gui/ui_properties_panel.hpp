#pragma once

#include <flecs.h>
#include <imgui.h>
#include <core/ui/widgets.hpp>

namespace atlas {

class ui_properties_panel {
public:
    ui_properties_panel() = delete;
    ui_properties_panel(flecs::world& p_world, flecs::entity& p_selected_entity);

    void on_ui_update();

private:
    void draw_component(flecs::entity component_entity, void* component_data);
    void draw_member(const flecs::entity& member_entity, void* base_ptr);

    flecs::world m_world;
    flecs::entity& m_selected_entity;
};

} // namespace atlas