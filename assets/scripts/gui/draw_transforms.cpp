#include <assets/scripts/gui/draw_transforms.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/ui/widgets.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/ext/matrix_transform.hpp>


draw_transforms::draw_transforms(flecs::world &p_registery) {

  m_registery = p_registery;

  m_query = p_registery.query_builder<atlas::transform>().build();

  atlas::attach(this, &draw_transforms::on_ui_update);
}

draw_transforms::~draw_transforms() {
  console_log_info("Editor Camera, Is Being Destroyed.\n");
}

void draw_transforms::on_ui_update() {
  m_registery.defer_begin();
  if (ImGui::Begin("Properties Panel")) {
    m_query.each([this](flecs::entity p_entity, atlas::transform &p_transform) {
      atlas::ui::draw_panel_component<atlas::rendertarget3d>(
          p_entity.name().c_str(), [&]() {
            atlas::ui::draw_vec3("pos 1", p_transform.Position);
            atlas::ui::draw_vec3("scale 1", p_transform.Scale);
            atlas::ui::draw_vec3("rotate 1", p_transform.Rotation);
            atlas::ui::draw_vec4("color 1", p_transform.Color);
            //    atlas::ui::button_open_file_dialog("Load Mesh 1",
            //                                       p_mesh.Filepath);

            //    if (p_mesh.Filepath != "") {
            //        std::filesystem::path relative_path =
            //          std::filesystem::relative(p_mesh.Filepath, "./");
            //        console_log_trace("Filepath = {}", p_mesh.Filepath);
            //        p_entity.set<atlas::rendertarget3d>(
            //          { relative_path.string() });
            //        p_mesh.Filepath = "";
            //    }
          });
    });
    ImGui::End();
  }
  m_registery.defer_end();
}