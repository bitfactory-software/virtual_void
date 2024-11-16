#pragma once

#include "../data/observer.h"
#include "../data/shared_const.h"
#include "../data/unique.h"
#include "../data/value.h"
#include "../open_method/via_type_info/factory.h"
#include "base.h"

namespace virtual_void::interface {

inline virtual_void::open_method::via_type_info::domain query_interface_domain;


template <is_virtual_void TO, is_virtual_void FROM = TO>
using copy_factory_method =
    virtual_void::open_method::via_type_info::factory<base<TO>(const FROM&)>;

template <typename CLASS, is_virtual_void TO, is_virtual_void FROM = TO>
copy_factory_method<TO, FROM> copy{query_interface_domain};

template <is_virtual_void TO, is_virtual_void FROM = TO>
using find_copy_factory_method =
    virtual_void::open_method::via_type_info::factory<
        copy_factory_method<TO, FROM> const&()>;

template <is_virtual_void TO, is_virtual_void FROM = TO>
find_copy_factory_method<TO, FROM> find_copy{query_interface_domain};


template <is_virtual_void TO, is_virtual_void FROM = TO>
using move_factory_method =
    virtual_void::open_method::via_type_info::factory<base<TO>(FROM&&)>;

template <typename CLASS, is_virtual_void TO, is_virtual_void FROM = TO>
move_factory_method<TO, FROM> move{query_interface_domain};

template <typename CLASS, is_virtual_void TO, is_virtual_void FROM = TO>
using find_move_factory_method =
    virtual_void::open_method::via_type_info::factory<
        move_factory_method<TO, FROM> const&()>;

template <is_virtual_void TO, is_virtual_void FROM = TO>
find_move_factory_method<TO, FROM> find_move{query_interface_domain};

template <template <is_virtual_void> typename INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
auto default_copy_to_interface() {
  return +[](FROM const& from) -> base<TO> {
    return INTERFACE<TO>{*unchecked_unerase_cast<CLASS>(from)};
  };
}
template <template <is_virtual_void> typename INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
auto default_copy_to_interface()
  requires std::copy_constructible<FROM> && std::same_as<TO, FROM>
{
  return +[](FROM const& from) -> base<TO> {
    typename INTERFACE<TO>::v_table_t* v_table =
        &INTERFACE<TO>::template imlpemented_v_table<CLASS>;
    return INTERFACE<TO>(from, v_table);
  };
};

template <template <is_virtual_void> typename TO_INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
void enable_interface_copy(auto impl) {
  using target_interface_t = TO_INTERFACE<TO>;
  copy<CLASS, TO, FROM>.define<target_interface_t>(impl);
  if (!find_copy<TO, FROM>.is_defined<CLASS>())
    find_copy<TO, FROM>.define<CLASS>(
        +[]() -> auto const& { return copy<CLASS, TO, FROM>; });
};

template <template <is_virtual_void> typename TO_INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
void enable_interface_copy() {
  enable_interface_copy<TO_INTERFACE, CLASS, TO, FROM>(
      default_copy_to_interface<TO_INTERFACE, CLASS, TO, FROM>());
}

template <template <is_virtual_void> typename INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
auto default_move_to_interface() {
  return +[](FROM&& from) -> base<TO> {
    typename INTERFACE<TO>::v_table_t* v_table =
        &INTERFACE<TO>::template imlpemented_v_table<CLASS>;
    return INTERFACE<TO>(std::move(from), v_table);
  };
}

template <template <is_virtual_void> typename TO_INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
void enable_interface_move(auto impl) {
  using target_interface_t = TO_INTERFACE<TO>;
  move<CLASS, TO, FROM>.define<target_interface_t>(impl);
  if (!find_move<TO, FROM>.is_defined<CLASS>())
    find_move<TO, FROM>.define<CLASS>(
        +[]() -> auto const& { return move<CLASS, TO, FROM>; });
};

template <template <is_virtual_void> typename TO_INTERFACE, typename CLASS,
          is_virtual_void TO, is_virtual_void FROM = TO>
void enable_interface_move() {
  enable_interface_move<TO_INTERFACE, CLASS, TO, FROM>(
      default_move_to_interface<TO_INTERFACE, CLASS, TO, FROM>());
}

template <typename TO_INTERFACE, typename FROM_INTERFACE>
TO_INTERFACE query_interface(const FROM_INTERFACE& from_interface) {
  using vv_to_t = typename TO_INTERFACE::virtual_void_t;
  using vv_from_t = typename FROM_INTERFACE::virtual_void_t;
  static_assert(is_virtual_void<vv_to_t>);
  static_assert(is_virtual_void<vv_from_t>);
  const auto& vv_from = get_virtual_void(from_interface);
  auto const& type_info = *get_meta(vv_from)->type_info();
  auto const& copy = find_copy<vv_to_t, vv_from_t>(type_info);
  base<vv_to_t> b = copy.construct<TO_INTERFACE>(vv_from);
  return std::move(static_v_table_cast<TO_INTERFACE>(std::move(b)));
}

template <typename TO_INTERFACE, typename FROM_INTERFACE>
TO_INTERFACE query_interface_moved(FROM_INTERFACE&& from_interface) {
  using vv_to_t = typename TO_INTERFACE::virtual_void_t;
  using vv_from_t = typename FROM_INTERFACE::virtual_void_t;
  static_assert(is_virtual_void<vv_to_t>);
  static_assert(is_virtual_void<vv_from_t>);
  auto vv_from = move_virtual_void(std::move(from_interface));
  auto const& type_info = *get_meta(vv_from)->type_info();
  auto const& move = find_move<vv_to_t, vv_from_t>(type_info);
  base<vv_to_t> b = move.construct<TO_INTERFACE>(std::move(vv_from));
  return std::move(static_v_table_cast<TO_INTERFACE>(std::move(b)));
}

};  // namespace virtual_void::interface