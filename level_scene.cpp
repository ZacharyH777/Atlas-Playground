
#include "level_scene.hpp"
#include <drivers/vulkan/helper_functions.hpp>
#include <drivers/vulkan/vulkan_context.hpp>
#include <drivers/vulkan/vulkan_swapchain.hpp>
#include <physics/jolt-cpp/jolt-imports.hpp>

#include <core/event/event.hpp>
#include <core/update_handlers/sync_update.hpp>

#include <core/serialize/serializer.hpp>
#include <core/ui/widgets.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <renderer/renderer.hpp>

#include <components/transform.hpp>
#include <physics/jolt-cpp/jolt_collision.hpp>
#include <physics/jolt-cpp/jolt_components.hpp>

#include <core/timer.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

// bool on_click_check = false;
//
// constexpr int ROTATION_DIRECTION_X = -1;
// constexpr int ROTATION_DIRECTION_Y = -1;
//
// struct MeshData {
//     glm::vec3 Position{ 0.f };
//     glm::vec3 Scale{ 0.f };
//     glm::vec3 Rotation{ 0.f };
//     std::string mesh_file = "";
//     glm::vec3 Color{ 1.f };
// };
//
// struct CameraData {
//     glm::vec3 Position{ 0.f };
//     glm::vec3 Front{ 0.f };
//     float Angle = 90.f;
// };
//
// static float sensitivity = 0.f;
//
// static MeshData sphere_data;
// static MeshData some_mesh_data;
// static CameraData camera_data;
// static std::string s_SceneFilepath = "";
// static glm::vec3 g_light_position = glm::vec3(0.0f, 0.0f, 1.0f);
//
// static void
// TraceImpl(const char* Message, ...) {
//     va_list list;
//     va_start(list, Message);
//     char buffer[1024];
//     vsnprintf(buffer, sizeof(buffer), Message, list);
//     va_end(list);
//     console_log_warn("TraceImpl Warning Occured!");
//     console_log_warn("{}", buffer);
// }
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
//
// level_scene::level_scene() {}
//

level_scene::level_scene(const std::string &p_tag) : atlas::scene_scope(p_tag) {

  console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);
  printf("Getting here\n");

  m_camera = this->create_new_object("camera");

  m_camera->add<atlas::physics::jolt::jolt_settings>();

  m_camera->add<atlas::camera>();

  m_sphere = this->create_new_object("sphere");

  m_sphere->set<atlas::rendertarget3d>(
      atlas::rendertarget3d("assets/models/colored_cube.obj"));

  m_sphere->add<atlas::physics::physics_body>();

  auto sphere_body = *m_sphere->get<atlas::physics::physics_body>();
  sphere_body.body_movement_type = atlas::physics::body_type::Dynamic;
  m_sphere->set(sphere_body);

  m_sphere->add<atlas::physics::collider_body>();
  m_sphere->add<atlas::transform_physics>();
  m_sphere->set<atlas::transform_physics>(
      {.position = {0.f, 2.10f, -7.30f},
       .quaterion_rotation{-0.5440211, 0, 0, -0.8390715},
       .scale = {.50f, .50f, .50f}});

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

  atlas::sync(this, &level_scene::on_update);

  //  query_objects.Position = { 0.0f, 1.50f, 0.0f };
  //  camera_data.Front = glm::vec3(-0.0f, 0.0f, -1.0f);

  // m_camera->add<atlas::camera>();

  //  sensitivity = m_camera->get<atlas::camera>()->MovementSpeed;
}
//    console_log_info("scene_scope::scene_scope with Tag = {} called!", p_tag);
//    atlas::world_scope world = atlas::system_registry::get_world();
//    std::string world_tag = world.get_tag();
//    console_log_trace("From level_scene::level_scene() ==> World Tag = {}",
//                      world_tag);
//
//    m_sphere = this->create_new_object("sphere");
//
//    m_sphere->set<atlas::RenderTarget3D>(
//      atlas::RenderTarget3D("assets/models/colored_cube.obj"));
//
//    m_sphere->set<atlas::Transform>({ .Position = { 0.f, 2.10f, -7.30f },
//                                      .Scale = { .20f, .20f, .20f },
//                                      .Color = { 1.0f, 1.f, 1.f, 1.f } });
//
//    m_sphere->add<atlas::RigidBody3D>();
//    m_sphere->add<atlas::Light>();
//
//    const atlas::Transform* transform = m_sphere->get<atlas::Transform>();
//
//    sphere_data.Position = transform->Position;
//    sphere_data.Scale = transform->Scale;
//    sphere_data.Rotation = transform->Rotation;
//
//    m_platform = this->create_new_object("mesh1");
//    m_platform->set<atlas::Transform>({
//      .Position = { 0.f, 1.40f, -7.4f },
//      .Scale = { 2.80f, -0.08f, 3.50f },
//    });
//
//    auto some_mesh_transform = m_platform->get<atlas::Transform>();
//    some_mesh_data.Position = some_mesh_transform->Position;
//    some_mesh_data.Scale = some_mesh_transform->Scale;
//    some_mesh_data.Rotation = some_mesh_transform->Rotation;
//    m_platform->set<atlas::RenderTarget3D>({ "assets/models/cube.obj" });
//
//
//    sync(this, &level_scene::on_update);
//    sync_physics(this, &level_scene::on_physics_update);
//    attach(this, &level_scene::on_ui_update);
//
//    console_log_warn("World Tag After Initialization ======>>>>> {}",
//                     atlas::system_registry::get_world().get_tag());
//}
//
// void
// level_scene::on_ui_update() {
//
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
//    if (ImGui::Begin("Properties Panel")) {
//
//        atlas::ui::draw_panel_component<atlas::RenderTarget3D>("Sphere", [&]()
//        {
//            atlas::ui::draw_vec3("pos 1", sphere_data.Position);
//            atlas::ui::draw_vec3("scale 1", sphere_data.Scale);
//            atlas::ui::draw_vec3("rotate 1", sphere_data.Rotation);
//            atlas::ui::draw_vec3("color 1", sphere_data.Color);
//            atlas::ui::draw_vec3("Light Pos", g_light_position);
//            atlas::ui::button_open_file_dialog("Load Mesh 1",
//                                               sphere_data.mesh_file);
//
//            if (sphere_data.mesh_file != "") {
//                std::filesystem::path relative_path =
//                  std::filesystem::relative(sphere_data.mesh_file, "./");
//                console_log_trace("Filepath = {}", sphere_data.mesh_file);
//                m_sphere->set<atlas::RenderTarget3D>(
//                  { relative_path.string() });
//                sphere_data.mesh_file = "";
//            }
//        });
//
//        atlas::ui::draw_panel_component<atlas::RenderTarget3D>(
//          "Some Mesh", [&]() {
//              atlas::ui::draw_vec3("Position 2", some_mesh_data.Position);
//              atlas::ui::draw_vec3("Scale 2", some_mesh_data.Scale);
//              atlas::ui::draw_vec3("Rotation 2", some_mesh_data.Rotation);
//              atlas::ui::draw_vec3("Color 2", some_mesh_data.Color);
//              atlas::ui::draw_float("Mouse Sensitivity", sensitivity);
//              atlas::ui::button_open_file_dialog("Load Mesh 2",
//                                                 some_mesh_data.mesh_file);
//          });
//
//
//
//        if (ImGui::Button("Save As")) {
//            std::string output_path = atlas::filesystem::SaveToFile("");
//            atlas::serializer serializer(this);
//            serializer.save_as(output_path);
//        }
//
//        ImGui::End();
//    }
//}
//
void level_scene::on_update() {

  if (!test_bool_2) {

    flecs::entity flecs_sphere = *m_sphere;
    atlas::physics::jolt_collision(flecs_sphere.id());
    engine = atlas::physics::initialize_engine(m_camera);
    test_bool_2 = true;
  }
  if (atlas::event::is_key_pressed(key_r) && !test_bool) {
    test_bool = true;
    engine->start_runtime();
  }
  if (test_bool) {
    engine->physics_step();
    engine->run_contact_add();
  }
  if (atlas::event::is_key_pressed(key_q) && test_bool) {
    test_bool = false;
    auto physics_sphere_transform = m_sphere->get<atlas::transform_physics>();
    m_sphere->set<atlas::transform>({.Position = {0.f, 2.10f, -7.30f},
                                     .Rotation{1.1504441, 0, 0},
                                     .Scale = {.50f, .50f, .50f},
                                     .Color = {1.0f, 1.f, 1.f, 1.f}});
    engine->stop_runtime();
  }

  auto camera_transform = *m_camera->get<atlas::transform>();
  auto camera_comp = *m_camera->get<atlas::camera>();
  float deltaTime;
  deltaTime = atlas::application::delta_time();
  if (atlas::event::is_key_pressed(key_f12)) {
    atlas::application::get_window().close();
  }

  if (atlas::event::is_key_pressed(key_w)) {
    camera_comp.process_keyboard(atlas::Forward, deltaTime);
  }
  if (atlas::event::is_key_pressed(key_s)) {
    camera_comp.process_keyboard(atlas::Backward, deltaTime);
  }
  if (atlas::event::is_key_pressed(key_a)) {
    camera_comp.process_keyboard(atlas::Left, deltaTime);
  }
  if (atlas::event::is_key_pressed(key_d)) {
    camera_comp.process_keyboard(atlas::Right, deltaTime);
  }
  if (atlas::event::is_key_pressed(key_left_shift)) {
    camera_comp.process_keyboard(atlas::Up, deltaTime);
  }
  if (atlas::event::is_key_pressed(key_space)) {
    camera_comp.process_keyboard(atlas::Down, deltaTime);
  }
  if (atlas::event::is_mouse_pressed(mouse_button_1)) {
    glm::vec2 cursor_pos = atlas::event::cursor_position();
    //! @note On right click make sure change starts as 0
    if (!on_click_check) {
      last_cursor_pos = cursor_pos;
      on_click_check = true;
    }

    //! @note offset is now delta_x and delta_y
    //! @note the difference between mouse old and new positions
    glm::vec2 offset;
    offset = cursor_pos - last_cursor_pos;

    glm::vec2 velocity;
    velocity = offset * (deltaTime * 4500);

    camera_comp.process_mouse_movement(velocity.x * -1, 0.0f);

    camera_comp.process_mouse_movement(0.0f, velocity.y * -1);

    last_cursor_pos = cursor_pos;
  } else {
    on_click_check = false;
  }
  camera_comp.MovementSpeed = 5;
  camera_comp.update_proj_view();
  camera_comp.IsMainCamera = true;

  m_camera->set<atlas::camera>(camera_comp);
  m_camera->set<atlas::transform>(camera_transform);
}
//     auto camera_transform = *m_camera->get<atlas::Transform>();

//     auto camera_comp = *m_camera->get<atlas::Camera>();

//     float deltaTime = atlas::sync_update::delta_time();

//     if (atlas::event::is_key_pressed(KEY_ESCAPE)) {
//         atlas::application::get_window().close();
//     }

//     if (atlas::event::is_key_pressed(KEY_W)) {
//         camera_comp.process_keyboard(atlas::FORWARD, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_S)) {
//         camera_comp.process_keyboard(atlas::BACKWARD, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_A)) {
//         camera_comp.process_keyboard(atlas::LEFT, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_D)) {
//         camera_comp.process_keyboard(atlas::RIGHT, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_LEFT_SHIFT)) {
//         camera_comp.process_keyboard(atlas::UP, deltaTime);
//     }
//     if (atlas::event::is_key_pressed(KEY_SPACE)) {
//         camera_comp.process_keyboard(atlas::DOWN, deltaTime);
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

//             camera_comp.ProcessMouseMovement(velocity.x *
//             ROTATION_DIRECTION_X, 0.0f);

//             camera_comp.ProcessMouseMovement(0.0f,velocity.y *
//             ROTATION_DIRECTION_Y);

//             last_cursor_pos = cursor_pos;
//         } else {
//             on_click_check = false;
//         }

//     camera_comp.MovementSpeed = 5;
//     camera_comp.UpdateProjView();
//     camera_comp.IsMainCamera = true;

//     m_camera->set<atlas::Camera>(camera_comp);
//     m_camera->set<atlas::Transform>(camera_transform);

// }
//
// void
// level_scene::on_physics_update() {
//
//    if (atlas::event::is_key_pressed(KEY_L)) {
//        m_is_simulation_enabled = true;
//    }
//
//    if (atlas::event::is_key_pressed(KEY_K)) {
//        m_is_simulation_enabled = false;
//    }
//
//    float steps = atlas::time::physcs_step();
//}
//