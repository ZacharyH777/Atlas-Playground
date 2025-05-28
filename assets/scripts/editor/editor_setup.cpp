#include <assets/scripts/editor/editor_setup.hpp>
#include <core/application.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/ext/matrix_transform.hpp>

editor_setup::editor_setup(flecs::world &p_registery) {

  m_registery = p_registery;

  m_query = p_registery.query_builder<editor_data>().build();

  atlas::sync(this, &editor_setup::on_update);
}

editor_setup::~editor_setup() {
  console_log_info("Editor Camera, Is Being Destroyed.\n");
}

void editor_setup::on_update() {
  m_registery.defer_begin();
  m_query.each([this](flecs::entity p_entity, editor_data p_data) {
    p_data.runtime_on = runtime_on;
    p_entity.set(p_data);
  });
  m_registery.defer_end();
}