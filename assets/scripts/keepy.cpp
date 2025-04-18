#include "keepy.hpp"
#include "assets/data/data.hpp"
#include <stdio.h>
#include <core/update_handlers/sync_update.hpp>
#include <core/event/event.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <core/application.hpp>
#include <core/system_framework/system_registry.hpp>

keepy::keepy() {
    printf("stuff\n");
    m_current_scene = atlas::system_registry::get_world("Editor World")->get_scene("LevelScene");
    atlas::attach(this, &keepy::on_ui_update);
    atlas::sync(this, &keepy::on_update);
    atlas::sync_physics(this, &keepy::on_physics_update);

    flecs::query<> query_objects = this->query_builder<atlas::camera>().build();
}

keepy::~keepy() {
    printf("zug\n");
}

void keepy::on_update() {
    m_current_scene->get_tag();

//  auto camera_transform = *m_camera->get<atlas::Transform>();

//  auto camera_comp = *m_camera->get<atlas::Camera>();

//     float deltaTime = atlas::sync_update::delta_time();

//   //  if (atlas::event::is_key_pressed(KEY_ESCAPE)) {
//   //      atlas::application::get_window().close();
//   //  }

//     if (atlas::event::is_key_pressed(key_w)) {
//         camera_comp.ProcessKeyboard(atlas::Forward, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_S)) {
//         camera_comp.ProcessKeyboard(atlas::BACKWARD, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_A)) {
//         camera_comp.ProcessKeyboard(atlas::LEFT, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_D)) {
//         camera_comp.ProcessKeyboard(atlas::RIGHT, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_LEFT_SHIFT)) {
//         camera_comp.ProcessKeyboard(atlas::UP, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_SPACE)) {
//         camera_comp.ProcessKeyboard(atlas::DOWN, deltaTime);
//     }

//     if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_RIGHT)) {
//         glm::vec2 cursor_pos = atlas::event::cursor_position();
            
//             //! @note On right click make sure change starts as 0
//             if(!on_click_check)
//             {
//                 last_cursor_pos = cursor_pos;
//                 on_click_check = true;
//             }

//             //! @note offset is now delta_x and delta_y
//             //! @note the difference between mouse old and new positions
//             glm::vec2 offset = cursor_pos - last_cursor_pos;

//             glm:glm::vec2 velocity = offset * (deltaTime * 100);

//             camera_comp.ProcessMouseMovement(velocity.x * ROTATION_DIRECTION_X, 0.0f);

//             camera_comp.ProcessMouseMovement(0.0f,velocity.y * ROTATION_DIRECTION_Y);

//             last_cursor_pos = cursor_pos;
//         } else {
//             on_click_check = false;
//         }

//     camera_comp.MovementSpeed = 5;
//     camera_comp.UpdateProjView();
//     camera_comp.IsMainCamera = true;

//     m_camera->set<atlas::Camera>(camera_comp);
//     m_camera->set<atlas::Transform>(camera_transform);

}

void keepy::on_physics_update() {}

void keepy::on_ui_update()
{
}