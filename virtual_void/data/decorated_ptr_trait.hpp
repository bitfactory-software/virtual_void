#pragma once

#include <virtual_void/virtual_void.hpp>

namespace virtual_void {

template <typename VV_VOID, typename META>
struct decorated_ptr_trait : virtual_void_default_unerase {
  using void_t = VV_VOID;
  using meta_t = META;
  static constexpr bool is_const = is_const_void<void_t>;

  static auto meta(const auto& ptr) { return ptr.get_meta(); }
  static bool has_value(const auto& ptr) { return static_cast<bool>(ptr.ptr_); }
};

}  // namespace virtual_void
