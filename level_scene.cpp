
#include "level_scene.hpp"
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>
#include <physics/jolt-cpp/jolt-imports.hpp>

#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>

#include <core/serialize/serializer.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <renderer/renderer.hpp>

#include <components/transform.hpp>
#include <physics/jolt-cpp/jolt_collision.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>

#include <core/timer.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

//
// namespace ui {
//     [[maybe_unused]] static bool BeginPopupContextWindow(const char* str_id,
//                                                          ImGuiMouseButton mb,
//                                                          bool over_items) {
//         return ImGui::BeginPopupContextWindow(
//           str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems));
//     }
//
// };

level_scene::level_scene(const std::string &p_tag) : atlas::scene_scope(p_tag) {

  console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);

  m_camera = this->create_new_object("Editor Camera");

  m_main_camera = this->create_new_object("Main Camera");
  m_main_camera->add<atlas::physics::jolt::jolt_settings>();

  m_sphere = this->create_new_object("sphere");

  m_sphere->set<atlas::rendertarget3d>(
      atlas::rendertarget3d("assets/models/colored_cube.obj"));

  m_sphere->add<atlas::physics::physics_body>();

  auto sphere_body = *m_sphere->get<atlas::physics::physics_body>();
  sphere_body.body_movement_type = atlas::physics::body_type::Dynamic;
  m_sphere->set(sphere_body);

  m_sphere->add<atlas::physics::collider_body>();
  // auto sphere_shape = *m_sphere->get<atlas::physics::collider_body>();
  // sphere_shape.shape_type = atlas::physics::collider_shape::Sphere;
  // m_sphere->set(sphere_shape);
  m_sphere->add<atlas::transform_physics>();
  m_sphere->set<atlas::transform_physics>(
      {.position = {0.f, 2.10f, -7.30f},
       .quaterion_rotation{-0.5440211, 0, 0, -0.8390715},
       .scale = {.30f, .30f, .30f}});

  m_platform = this->create_new_object("platform");

  m_platform->add<atlas::physics::collider_body>();
  m_platform->add<atlas::transform_physics>();

  m_sphere->set<atlas::transform>({.Position = {0.f, 2.10f, -7.30f},
                                   .Rotation{1.1504441, 0, 0},
                                   .Scale = {.50f, .50f, .50f},
                                   .Color = {1.0f, 1.f, 1.f, 1.f}});

  auto platform_body = *m_platform->get<atlas::physics::collider_body>();
  platform_body.shape_type = atlas::physics::collider_shape::Box;
  platform_body.half_extents = {10, 1, 10};
  m_platform->set(platform_body);

  m_platform->set<atlas::rendertarget3d>(
      atlas::rendertarget3d("assets/models/cube.obj"));

  auto platform_location = *m_platform->get<atlas::transform_physics>();
  platform_location.position = {0, -10, 0};
  m_platform->set(platform_location);

  m_platform->set<atlas::transform>(
      {.Position = {0.f, -10.0f, 0.0f}, .Scale = {10.0f, 1.00f, 10.0f}});

  atlas::sync_physics(this, &level_scene::on_physics_update);
}

//! @remark This is what you called pre_init. Probably should be a function for
//! this because each scene needs one and they cannot not be done in the
//! constructor as they require access to scenes through world.
void level_scene::initialize() {
  test_bool_2 = true; // Run only once for now
  flecs::world registery = *this;
  // Create actions
  m_editor_camera = atlas::create_ref<editor_camera>(registery);
  m_editor_camera->attach_entity(m_camera, registery);

  m_main_controls = atlas::create_ref<main_camera>(registery);
  m_main_controls->attach_entity(m_main_camera, registery);

  m_camera_manager = atlas::create_ref<camera_manager>(registery);
  m_camera_manager->attach_entity(m_camera, registery);
  m_camera_manager->attach_entity(m_main_camera, registery);

  m_main_camera->set<camera_data>({.type = camera_type::MAIN_CAMERA});

  m_editor_setup = atlas::create_ref<editor_setup>(registery);
  m_draw_transforms = atlas::create_ref<draw_transforms>(registery);

  // Add collisions defualt or user defined
  atlas::physics::jolt_collision(((flecs::entity)*m_sphere).id());

  // Start the engine on runtime start
  // This wil not need to be in initialize once scene are more organized
  engine = atlas::physics::initialize_engine(m_main_camera);
}

// These functions will not need to be in level scene either once scenes are
// organized and we have something like load_scene and run_scene elsewhere.
void level_scene::start_runtime() {
  test_bool = true;
  m_editor_setup->runtime_on = true;
  engine->start_runtime();
}

void level_scene::stop_runtime() {
  test_bool = false;
  m_editor_setup->runtime_on = false;
  // Resets the positions of the objects
  //! @bug (fake and will not work after transform_physics becomes transform)
  auto physics_sphere_transform = m_sphere->get<atlas::transform_physics>();
  m_sphere->set<atlas::transform>({.Position = {0.f, 2.10f, -7.30f},
                                   .Rotation{1.1504441, 0, 0},
                                   .Scale = {.50f, .50f, .50f},
                                   .Color = {1.0f, 1.f, 1.f, 1.f}});
  engine->stop_runtime();
}

//    if (ImGui::Begin("Viewport")) {
//        glm::vec2 viewportPanelSize =
//          glm::vec2{ atlas::application::get_window().get_width(),
//                     atlas::application::get_window().get_height() };
//
//        if (ImGui::Begin("Image")) {
//            ImGui::End();
//        }
//        ImGui::End();
//    }
//
//    if (ImGui::Begin("Settings")) {
//        ImGui::End();
//    }
//
//    if (ImGui::Begin("Scene Heirarchy")) {
//
//        if (ui::BeginPopupContextWindow(0, 1, false)) {
//            if (ImGui::MenuItem("Create Empty Entity")) {
//
//            }
//            ImGui::EndPopup();
//        }
//        ImGui::End();
//    }
//
//    if (ImGui::Begin("Content Browser")) {
//        ImGui::End();
//    }
//

void level_scene::on_physics_update() {

  /***************************Physics Start****************************
   *
   * This is the physics functions required to make the engine work.
   * There is no runtime call yet, so as of right now this is the only
   * option until we fix scene usage.
   *
   ********************************************************************/

  if (!test_bool_2) {
    initialize();
  }

  // Physics runtime toggle on
  if (atlas::event::is_key_pressed(key_r) && !test_bool) {
    start_runtime();
  }

  /**
   * @warning  All physics based actions should be run here. Including all
   * actions that run on this event. There will be some collision defining
   * physics_update() in different classes right now. But can't fix that until
   * we have a function for:
   *  init,
   *  start_runtime,
   *  physics_run,
   *  end_runtime.
   */

  // Physics time step
  if (test_bool) {
    engine->physics_step();
    engine->run_contact_add();
  }

  // Phsyics runtime toggle off
  if (atlas::event::is_key_pressed(key_q) && test_bool) {
    stop_runtime();
  }
}