#pragma once

#include "../shared_const_trait.h"
#include "meta.h"

namespace virtual_void::erased::data::has_m_table {
using shared_const_data_ptr = shared_const_ptr<with_m_table>;
}

namespace virtual_void::erased {
template <>
struct data_trait<erased::data::has_m_table::shared_const_data_ptr>
    : shared_const_trait<erased::data::has_m_table::has_m_table> {};
}  // namespace virtual_void::erased

namespace virtual_void::erased::data::has_m_table {

static_assert(is_data_pointer<shared_const_data_ptr>);

using shared_const = virtual_void<shared_const_data_ptr>;
template <typename T>
using typed_shared_const = virtual_typed<T const, shared_const_data_ptr>;

template <typename T, typename... ARGS>
auto make_shared_const(ARGS&&... args) {
  return erased::data::make_shared_const<holder<const T, has_m_table>>(
      std::forward<ARGS>(args)...);
}

static_assert(erased::is_virtual_void<shared_const>);
static_assert(erased::is_virtual_void<typed_shared_const<int>>);

}  // namespace virtual_void::erased::data::has_m_table