#pragma once

#include "../../forward.h"

namespace virtual_void::data::has_no_meta {

struct meta {
  const auto* get_meta() const { return this; }
  template <typename T>
  meta(std::in_place_type_t<T>) {}
  meta() = default;
  type_info_ptr type_info() const { return {}; }
  DATA_ALIGNED_DESRTUCTOR_VIRTUAL ~meta() = default;
};

}  // namespace virtual_void::data::has_no_meta