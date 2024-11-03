#pragma once

#include "../../virtual_void.h"
#include "../../utillities/VIRTUAL_DESTRUCTOR_FOR_DEBUGGING.h"


namespace virtual_void::data::has_no_meta {

struct meta {
  const auto* get_meta() const { return this; }
  template <typename T>
  meta(std::in_place_type_t<T>) {}
  template <is_meta META>
  meta(const META&) {}
  meta() = default;
  type_info_ptr type_info() const { return {}; }
  VIRTUAL_DESTRUCTOR_FOR_DEBUGGING ~meta() = default;
};

}  // namespace virtual_void::data::has_no_meta