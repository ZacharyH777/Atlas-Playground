#include "requirements.hpp"

namespace atlas {
void requirements::apply(flecs::entity p_entity) const {
  for (const auto &builder : builders) {
    builder(p_entity);
  }
}

bool requirements::does_exist(flecs::entity p_entity, flecs::world &p_registery,
                              flecs::id_t p_component_id) {
  return p_entity.has(p_registery.pair(p_registery.id<runs>(), p_component_id));
}

void requirements::clear() {
  builders.clear();
  requirement_ids.clear();
}
} // namespace atlas