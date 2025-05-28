#include <assets/scripts/camera/camera_manager.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/ext/matrix_transform.hpp>

camera_manager::camera_manager(flecs::world &p_registery) {

  m_registery = p_registery;

  m_query = p_registery.query_builder<atlas::camera, camera_data, editor_data>().build();

  // List structs that are required by this action ex: physics_body, transform
  m_requirements
      .add<flecs::pair<atlas::requirements::runs, camera_manager_action>>(
          p_registery);

  m_requirements.add<camera_data>(p_registery);
  m_requirements.add<atlas::camera>(p_registery);
  m_requirements.add<editor_data>(p_registery);

  atlas::sync(this, &camera_manager::on_update);
}

void camera_manager::attach_entity(atlas::ref<atlas::scene_object> p_entity,
                                   flecs::world &p_registery) {
  if (!m_requirements.does_exist(*p_entity, p_registery,
                                 p_registery.id<camera_manager_action>())) {

    m_requirements.apply(*p_entity);
  }
}

camera_manager::~camera_manager() {
  console_log_info("Editor Camera, Is Being Destroyed.\n");
}

void camera_manager::on_update() {
  m_registery.defer_begin();
  m_query.each([](flecs::entity p_entity, atlas::camera camera_comp,
                      camera_data p_camera_data, editor_data p_data) {
    if (p_data.runtime_on && p_camera_data.type == camera_type::MAIN_CAMERA) {
      camera_comp.IsMainCamera = true;
      camera_comp.update_proj_view();
    } else if (!p_data.runtime_on &&
               p_camera_data.type == camera_type::EDITOR_CAMERA) {
      camera_comp.IsMainCamera = true;
      camera_comp.update_proj_view();
    } else {
      camera_comp.IsMainCamera = false;
    }

    p_entity.set(camera_comp);
  });
  m_registery.defer_end();
}