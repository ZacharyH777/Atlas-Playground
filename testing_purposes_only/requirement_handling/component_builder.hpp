#pragma once

#include "offset_of.hpp" // This does not work but maybe some can figure it out.
#include <core/engine_logger.hpp>
#include <cstring>
#include <flecs.h>
#include <flecs/addons/meta.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_set>

namespace atlas {
class component_builder {
public:
  component_builder(flecs::world &p_world) {
    m_world = p_world;
    m_offset = 0;
  }

  template <typename StructType>
  component_builder &create(const std::string &p_name) {
    flecs::entity existing = m_world.lookup(p_name.c_str());
    if (existing.is_valid()) {
      console_log_fatal("Component already exists: {}", p_name);
      return *this;
    }

    m_struct = m_world.component<StructType>(p_name.c_str());

    m_struct.set<flecs::Struct>({});
    m_offset = 0;
    m_fields.clear();

    return *this;
  }

  template <typename StructType, typename MemberType>
  component_builder &member(const std::string &p_name,
                            MemberType StructType::*member) {

    if (!m_struct.is_valid()) {
      console_log_fatal("Call create() before member()");
      return *this;
    }

    if (m_fields.contains(p_name)) {
      console_log_fatal("Member {}, already added.", p_name);
      return *this;
    }

    size_t offset = reinterpret_cast<std::size_t>(
        &(reinterpret_cast<StructType *>(0)->*member));
    flecs::entity member_type = m_world.component<MemberType>();

    if (!member_type.has<flecs::Struct>() &&
        !member_type.has<flecs::Primitive>()) {
      member_type.set<flecs::Struct>({});
    };

    flecs::entity member_base = m_world.lookup(p_name.c_str());
    if (!member_base.is_valid()) {
      member_base = m_world.entity(p_name.c_str());
      member_base.set<flecs::Member>({.type = member_type.id(),
                                      .count = 0,
                                      .unit = 0,
                                      .offset = static_cast<int32_t>(offset),
                                      .use_offset = true});
    }

    m_struct.add(member_base);
    m_fields.insert(p_name);
    return *this;
  }

  flecs::entity build() { return m_struct; }

private:
  flecs::entity m_struct;
  flecs::world m_world;
  size_t m_offset = 0;
  std::unordered_set<std::string> m_fields;
};
}; // namespace atlas