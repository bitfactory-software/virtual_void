#pragma once

#include "../unique_trait.h"
#include "meta.h"

namespace virtual_void::erased::data::has_type_info {
using unique_data =
    erased::data::unique_ptr<allocation_base<meta>>;
}

namespace virtual_void::erased {
template <>
struct virtual_void_trait<data::has_type_info::unique_data>
    : unique_trait<data::has_type_info::meta> {};

}  // namespace virtual_void::erased

namespace virtual_void::erased::data::has_type_info {
using unique = erased::virtual_void<unique_data>;
template <typename T>
using typed_unique = erased::virtual_typed<T, unique_data>;
static_assert(erased::is_virtual_void<unique>);
static_assert(erased::is_virtual_void<typed_unique<int> >);
}  // namespace virtual_void::erased::data::has_type_info