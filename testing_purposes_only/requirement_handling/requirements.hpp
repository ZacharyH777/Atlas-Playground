#pragma once

#include <flecs.h>
#include <type_traits>
#include <unordered_set>
#include <vector>

namespace atlas {

class requirements {
public:
  template <typename T> void add(flecs::world& p_world) {
    static_assert(std::is_class_v<T>, "Only struct types allowed.");

    // Register a "parentless" component and get the id
    flecs::id_t component_id = p_world.id<T>();
    if (requirement_ids.insert(component_id).second) {

      // &requirements::ensure_component<T> is needed because we don't want
      // to bind the call to this class but instead treat like a static call.
      builders.emplace_back(&requirements::enforce_component<T>);
    }
  }

  bool does_exist(flecs::entity entity, flecs::world& world, flecs::id_t component_id);

  void apply_to(flecs::entity p_entity) const;

  void clear();

  struct runs{};

private:
  std::unordered_set<flecs::id_t> requirement_ids;
  std::vector<void (*)(flecs::entity)> builders;

  template <typename T> static void enforce_component(flecs::entity p_entity) {
    if (!p_entity.has<T>()) {
      p_entity.add<T>();
    }
  }

};

}; // namespace atlas