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
#include <physics/physics_3d/jolt/jolt_helper.hpp>
#include <testing_purposes_only/requirement_handling/component_builder.hpp>

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
  flecs::world registery = *this;

  // Turns out flecs has an is_a function. That does this.
  is = registery.entity("is");
  object = registery.entity("object");

  registery.component<float>("float");
  registery.component<int>("int");
  registery.component<bool>("bool");
  registery.component<glm::vec3>("glm::vec3");
  registery.component<glm::vec4>("glm::vec4");
  registery.component<glm::highp_vec3>("glm::highp_vec3");
  registery.component<glm::highp_vec4>("glm::highp_vec4");

  // Tetsing reflection builder

  atlas::component_builder builder(registery);

  builder.create<atlas::transform>("transform")
      .member("Position", &atlas::transform::Position)
      .member("Rotation", &atlas::transform::Rotation)
      .member("Scale", &atlas::transform::Scale)
      .member("Color", &atlas::transform::Color);

  builder.create<editor_controls>("camera controls")
      .member("down", &editor_controls::down)
      .member("up", &editor_controls::up)
      .member("left", &editor_controls::left)
      .member("right", &editor_controls::right)
      .member("forward", &editor_controls::forward)
      .member("backward", &editor_controls::backward)
      .member("player", &editor_controls::player);

  builder.create<atlas::physics::physics_body>("RigidBody")
      .member("Velocity", &atlas::physics::physics_body::linear_velocity)
      .member("Angular Velocity",
              &atlas::physics::physics_body::angular_velocity)
      .member("Force Vector", &atlas::physics::physics_body::cumulative_force)
      .member("Torque Vector", &atlas::physics::physics_body::cumulative_torque)
      .member("Friction", &atlas::physics::physics_body::friction)
      .member("Restitution", &atlas::physics::physics_body::restitution)
      .member("Use Gravity", &atlas::physics::physics_body::use_gravity)
      .member("Gravity Mult", &atlas::physics::physics_body::gravity_factor);

  object_test =
      builder.create<atlas::transform_physics>("physics_transform")
          .member("Position", &atlas::transform_physics::position)
          .member("Rotation", &atlas::transform_physics::rotation)
          .member("Scale", &atlas::transform_physics::scale)
          .build(); // Just a showcase .build is only for serialization
  // We can get into why this is good for serialization later.

  this->query_builder<atlas::transform>().build();
  m_camera = this->create_new_object("Editor Camera");
  flecs::entity temp;
  temp = *m_camera;
  temp.add(is, object);

  m_main_camera = this->create_new_object("Main Camera");
  temp = *m_main_camera;
  temp.add(is, object);
  m_main_camera->add<atlas::physics::jolt_settings>();
  m_main_camera->add<atlas::physics::jolt_config>();

  m_sphere = this->create_new_object("Cube");
  temp = *m_sphere;
  temp.add(is, object);

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
  m_sphere->set<atlas::transform_physics>({.position = {0.f, 2.10f, -7.30f},
                                           .rotation = {-.30f, 0.0f, 0.0f},
                                           .scale = {.30f, .30f, .30f}});

  m_platform = this->create_new_object("Platform");
  temp = *m_platform;
  temp.add(is, object);

  m_test_object = this->create_new_object("Test Child Object");
  temp = *m_test_object;
  temp.add(is, object);

  flecs::entity temp2 = *m_platform;
  temp.child_of(temp2.id());

  m_test_object2 = this->create_new_object("Test Child Object 2");
  temp = *m_test_object2;
  temp.add(is, object);

  temp.child_of(temp2.id());

  m_test_object3 = this->create_new_object("Test GrandChild Object");
  temp = *m_test_object3;
  temp.add(is, object);

  temp2 = *m_test_object;
  temp.child_of(temp2.id());

  m_player_head = this->create_new_object("Player Head");
  temp = *m_player_head;
  temp.add(is, object);

  m_player_torso = this->create_new_object("Player Torso");
  temp = *m_player_torso;
  temp.add(is, object);

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
  // m_draw_transforms = atlas::create_ref<draw_transforms>(registery);

  m_ui_hierarchy = atlas::create_ref<ui_hierarchy>(registery, test);
  m_ui_properties =
      atlas::create_ref<atlas::ui_properties_panel>(registery, test);

  // Add collisions defualt or user defined
  atlas::physics::jolt_collision(((flecs::entity)*m_sphere).id());

  // Start the engine on runtime start
  // This wil not need to be in initialize once scene are more organized
  engine = atlas::physics::initialize_engine(m_main_camera, registery);
}

// These functions will not need to be in level scene either once scenes are
// organized and we have something like load_scene and run_scene elsewhere.
void level_scene::start_runtime() {
  test_bool = true;
  m_editor_setup->runtime_on = true;
  auto physics_sphere_transform = m_sphere->get<atlas::transform_physics>();

  auto quaternion = glm::quat({physics_sphere_transform->rotation.x,
                               physics_sphere_transform->rotation.y,
                               physics_sphere_transform->rotation.z});

  m_sphere->set<atlas::transform>(
      {.Position = physics_sphere_transform->position,
       .QuaternionRotation = glm::highp_vec4(quaternion.x, quaternion.y,
                                             quaternion.z, quaternion.w),
       .Rotation = physics_sphere_transform->rotation,
       .Scale = physics_sphere_transform->scale,
       .Color = {1.0f, 1.f, 1.f, 1.f}});
  engine->start_runtime();
}

void level_scene::stop_runtime() {
  test_bool = false;
  m_editor_setup->runtime_on = false;
  // Resets the positions of the objects
  //! @bug (fake and will not work after transform_physics becomes transform)
  auto physics_sphere_transform = m_sphere->get<atlas::transform_physics>();

  auto quaternion = glm::quat({physics_sphere_transform->rotation.x,
                               physics_sphere_transform->rotation.y,
                               physics_sphere_transform->rotation.z});

  m_sphere->set<atlas::transform>(
      {.Position = physics_sphere_transform->position,
       .QuaternionRotation = glm::highp_vec4(quaternion.x, quaternion.y,
                                             quaternion.z, quaternion.w),
       .Rotation = physics_sphere_transform->rotation,
       .Scale = physics_sphere_transform->scale,
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

  if (atlas::event::is_key_pressed(key_j)) {
    // console_log_info("key_j");
    glm::vec3 move = {0.1, 0, 0};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }
  if (atlas::event::is_key_pressed(key_k)) {
    // console_log_info("key_k");
    glm::vec3 move = {-0.1, 0, 0};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }
  if (atlas::event::is_key_pressed(key_h)) {
    // console_log_info("key_h");
    glm::vec3 move = {0, 0, 0.1};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }
  if (atlas::event::is_key_pressed(key_l)) {
    // console_log_info("key_l");
    glm::vec3 move = {0, 0, -0.1};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }
  if (atlas::event::is_key_pressed(key_y)) {
    // console_log_info("key_l");
    glm::vec3 move = {0, -0.1, 0};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }
  if (atlas::event::is_key_pressed(key_u)) {
    // console_log_info("key_l");
    glm::vec3 move = {0, 0.1, 0};
    // atlas::physics::add_force(move,
    //                           *(m_sphere->get<atlas::physics::physics_body>()));
    atlas::physics::add_velocity(
        move, *(m_sphere->get<atlas::physics::physics_body>()));
  }

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
