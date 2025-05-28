#include <assets/scripts/camera/editor_camera.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/ext/matrix_transform.hpp>

editor_camera::editor_camera(flecs::world &p_registery) {

  m_registery = p_registery;

  m_query =
      p_registery
          .query_builder<atlas::transform, atlas::camera, editor_controls>()
          .with<atlas::requirements::runs>()
          .second<editor_camera_action>()
          .build();

  // List structs that are required by this action ex: physics_body, transform
  m_requirements
      .add<flecs::pair<atlas::requirements::runs, editor_camera_action>>(
          p_registery);

  m_requirements.add<editor_controls>(p_registery);
  m_requirements.add<atlas::camera>(p_registery);

  atlas::sync(this, &editor_camera::on_update);
}

void editor_camera::attach_entity(atlas::ref<atlas::scene_object> p_entity,
                                  flecs::world &p_registery) {
  if (!m_requirements.does_exist(*p_entity, p_registery,
                                 p_registery.id<editor_camera_action>())) {

    m_requirements.apply_to(*p_entity);
  }
}

editor_camera::~editor_camera() {
  console_log_info("Editor Camera, Is Being Destroyed.\n");
}

void editor_camera::on_update() {
  m_registery.defer_begin();
  m_query.each([](flecs::entity p_entity, atlas::transform camera_transform,
                  atlas::camera camera_comp, editor_controls p_controls) {

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
    camera_comp.update_proj_view();
    camera_comp.IsMainCamera = true;

    p_entity.set<atlas::camera>(camera_comp);
    p_entity.set<atlas::transform>(camera_transform);
    p_entity.set<editor_controls>(p_controls);
  });
  m_registery.defer_end();
  // auto camera_transform = *m_camera->get<atlas::transform>();
  // auto camera_comp = *m_camera->get<atlas::camera>();
  // float delta_time;
  // delta_time = atlas::application::delta_time();
  // if (atlas::event::is_key_pressed(key_f12)) {
  //   atlas::application::get_window().close();
  // }

  // if (atlas::event::is_key_pressed(key_w)) {
  //   camera_comp.process_keyboard(atlas::Forward, delta_time);
  // }
  // if (atlas::event::is_key_pressed(key_s)) {
  //   camera_comp.process_keyboard(atlas::Backward, delta_time);
  // }
  // if (atlas::event::is_key_pressed(key_a)) {
  //   camera_comp.process_keyboard(atlas::Left, delta_time);
  // }
  // if (atlas::event::is_key_pressed(key_d)) {
  //   camera_comp.process_keyboard(atlas::Right, delta_time);
  // }
  // if (atlas::event::is_key_pressed(key_left_shift)) {
  //   camera_comp.process_keyboard(atlas::Up, delta_time);
  // }
  // if (atlas::event::is_key_pressed(key_space)) {
  //   camera_comp.process_keyboard(atlas::Down, delta_time);
  // }
  // if (atlas::event::is_mouse_pressed(mouse_button_1)) {
  //   glm::vec2 cursor_pos = atlas::event::cursor_position();
  //   //! @note On right click make sure change starts as 0
  //   if (!on_click_check) {
  //     last_cursor_pos = cursor_pos;
  //     on_click_check = true;
  //   }

  //   //! @note offset is now delta_x and delta_y
  //   //! @note the difference between mouse old and new positions
  //   glm::vec2 offset;
  //   offset = cursor_pos - last_cursor_pos;

  //   glm::vec2 velocity;
  //   velocity = offset * (delta_time * 4500);

  //   camera_comp.process_mouse_movement(velocity.x * -1, 0.0f);

  //   camera_comp.process_mouse_movement(0.0f, velocity.y * -1);

  //   last_cursor_pos = cursor_pos;
  // } else {
  //   on_click_check = false;
  // }
  // camera_comp.MovementSpeed = 5;
  // camera_comp.update_proj_view();
  // camera_comp.IsMainCamera = true;

  // m_camera->set<atlas::camera>(camera_comp);
  // m_camera->set<atlas::transform>(camera_transform);
}