#pragma once

#include <typeindex>

namespace virtual_void {
using type_info_ptr = std::type_info const*;

template <typename VOID>
bool is_const_void;
template <>
constexpr bool is_const_void<void*> = false;
template <>
constexpr bool is_const_void<void const*> = true;

template <typename>
struct self_pointer;
template <>
struct self_pointer<void*> {
  template <typename CLASS>
  using type = CLASS*;
};
template <>
struct self_pointer<const void*> {
  template <typename CLASS>
  using type = const CLASS*;
};

class error;

}  // namespace virtual_void

#ifdef _DEBUG
#define DATA_ALIGNED_DESRTUCTOR_VIRTUAL virtual
#else
#define DATA_ALIGNED_DESRTUCTOR_VIRTUAL
#endif  // DEBUG
