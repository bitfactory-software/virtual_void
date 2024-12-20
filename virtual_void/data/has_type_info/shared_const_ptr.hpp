#pragma once

#include <virtual_void/data/shared_const_ptr_trait.hpp>
#include <virtual_void/data/has_type_info/meta.hpp>

namespace virtual_void::data::has_type_info {
using shared_const_ptr = data::shared_const_ptr<meta>;
}

namespace virtual_void {
template <>
struct virtual_void_trait<data::has_type_info::shared_const_ptr>
    : shared_const_ptr_trait<data::has_type_info::meta> {};
}  // namespace virtual_void

namespace virtual_void::data::has_type_info {
static_assert(has_virtual_void_trait<shared_const_ptr>);
template <typename T>
using typed_shared_const_ptr = virtual_typed<T const, shared_const_ptr>;
static_assert(is_virtual_void<shared_const_ptr>);
static_assert(is_virtual_typed<typed_shared_const_ptr<int>>);
}  // namespace virtual_void::data::has_type_info