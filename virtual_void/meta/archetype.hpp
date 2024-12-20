#pragma once

#include <virtual_void/meta/forward.hpp>
#include <virtual_void/utillities/VV_EXPORT.hpp>
#include <virtual_void/virtual_void.hpp>

namespace virtual_void::meta {

VV_EXPORT int next_archetype_index_value();

class archetype_t {
  const int index_ = next_archetype_index_value();

 public:
  archetype_t() = default;
  archetype_t(const archetype_t&) = delete;
  constexpr int get_archetype_index() { return index_; }
  int interface_count_ = 0;
  int open_method_count_ = 0;
};

template <typename ARCHETYPE>
archetype_t& archetype_instance();

template <typename ARCHETYPE>
archetype_t& archetype_implementation() {
  static archetype_t archetype_;
  return archetype_;
}

struct archetype_unspecified;
template <>
VV_EXPORT archetype_t& archetype_instance<archetype_unspecified>();

template <typename CLASS>
struct archetype_for_class : std::type_identity<archetype_unspecified> {};

template <typename CLASS>
using archetype_for_class_t = archetype_for_class<CLASS>::type;

template <typename CLASS>
archetype_t& archetype_of() {
  return archetype_instance<archetype_for_class_t<CLASS>>();
}

}  // namespace virtual_void::meta