#include <assets/scripts/physics/rigid_body.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/ext/matrix_transform.hpp>

rigid_body::rigid_body(flecs::world &p_registery) {

  m_registery = p_registery;

  m_query = p_registery
                .query_builder<atlas::transform, atlas::camera, editor_controls,
                               editor_data>()
                .with<atlas::requirements::runs>()
                .second<rigid_body_action>()
                .build();

  // List structs that are required by this action ex: physics_body, transform
  m_requirements
      .add<flecs::pair<atlas::requirements::runs, rigid_body_action>>(
          p_registery);

  m_requirements.add<editor_controls>(p_registery);
  m_requirements.add<atlas::camera>(p_registery);
  m_requirements.add<editor_data>(p_registery);

  atlas::sync(this, &rigid_body::on_update);
}

void rigid_body::attach_entity(atlas::ref<atlas::scene_object> p_entity,
                                  flecs::world &p_registery) {
  if (!m_requirements.does_exist(*p_entity, p_registery,
                                 p_registery.id<rigid_body_action>())) {

    m_requirements.apply(*p_entity);
  }
}

rigid_body::~rigid_body() {
  console_log_info("Editor Camera, Is Being Destroyed.\n");
}

void rigid_body::on_update() {
  m_registery.defer_begin();
  m_query.each([](flecs::entity p_entity, atlas::transform camera_transform,
                  atlas::camera camera_comp, editor_controls p_controls,
                  editor_data p_data) {
    
    // Camera functionality starts here
    if (!p_data.runtime_on) {
      float delta_time;
      delta_time = atlas::application::delta_time();

      if (atlas::event::is_key_pressed(key_f12)) {
        atlas::application::get_window().close();
      }

      if (atlas::event::is_key_pressed(p_controls.forward)) {
        camera_comp.process_keyboard(atlas::Forward, delta_time);
      }
      if (atlas::event::is_key_pressed(p_controls.backward)) {
        camera_comp.process_keyboard(atlas::Backward, delta_time);
      }
      if (atlas::event::is_key_pressed(p_controls.left)) {
        camera_comp.process_keyboard(atlas::Left, delta_time);
      }
      if (atlas::event::is_key_pressed(p_controls.right)) {
        camera_comp.process_keyboard(atlas::Right, delta_time);
      }
      if (atlas::event::is_key_pressed(p_controls.down)) {
        camera_comp.process_keyboard(atlas::Up, delta_time);
      }
      if (atlas::event::is_key_pressed(p_controls.up)) {
        camera_comp.process_keyboard(atlas::Down, delta_time);
      }
      if (atlas::event::is_mouse_pressed(mouse_button_1)) {
        glm::vec2 cursor_pos = atlas::event::cursor_position();
        //! @note On right click make sure change starts as 0
        if (!p_controls.on_click_check) {
          p_controls.last_cursor_pos = cursor_pos;
          p_controls.on_click_check = true;
        }

        //! @note offset is now delta_x and delta_y
        //! @note the difference between mouse old and new positions
        glm::vec2 offset;
        offset = cursor_pos - p_controls.last_cursor_pos;

        glm::vec2 velocity;
        velocity = offset * (delta_time * 4500);

        camera_comp.process_mouse_movement(velocity.x * -1, 0.0f);

        camera_comp.process_mouse_movement(0.0f, velocity.y * -1);

        p_controls.last_cursor_pos = cursor_pos;
      } else {
        p_controls.on_click_check = false;
      }
      camera_comp.MovementSpeed = 5;

      p_entity.set<atlas::camera>(camera_comp);
      p_entity.set<atlas::transform>(camera_transform);
      p_entity.set<editor_controls>(p_controls);
    }
  });
  m_registery.defer_end();
}