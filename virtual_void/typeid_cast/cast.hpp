#pragma once

#include <virtual_void/data/has_type_info/observer.hpp>
#include <virtual_void/open_method/algorithm.hpp>
#include <virtual_void/virtual_void.hpp>

namespace virtual_void::typeid_cast {

template <template <typename> typename CONST, typename FOUND, typename FROM>
auto cast_implementation_(auto* from, const std::type_info& to) {
  typename CONST<void>::type found = nullptr;
  meta::visit_class<FROM>(
      overload{[&]<typename C> {
                 if (!found && typeid_of<FROM>() == to)
                   found = static_cast<CONST<void>::type>(from);
               },
               [&]<typename, typename B> {
                 if (!found)
                   found = cast_implementation_<CONST, FOUND, B>(
                       static_cast<CONST<B>::type>(from), to);
               }});
  return found;
}
struct const_cast_implementation {
  template <typename T>
  struct const_ {
    using type = const T*;
  };
  template <typename FROM>
  auto operator()(const FROM* from, const std::type_info& to) {
    return cast_implementation_<const_, data::has_type_info::const_observer,
                                FROM>(from, to);
  }
};
struct cast_implementation {
  template <typename T>
  struct non_const_ {
    using type = T;
  };
  template <typename FROM>
  auto operator()(FROM* from, const std::type_info& to) {
    return cast_implementation_<non_const_,
                                data::has_type_info::mutable_observer, FROM>(
        from, to);
  }
};
template <template <typename SIG> typename OPEN_METHOD>
using const_cast_method =
    OPEN_METHOD<void const*(const_, const std::type_info& to)>;
template <template <typename SIG> typename OPEN_METHOD>
using cast_method = OPEN_METHOD<void*(const void*, const std::type_info& to)>;
void fill_const_cast_for(auto classes, auto& method) {
  open_method::fill_with_overloads(classes, method,
                                   const_cast_implementation{});
}
template <typename... CLASSES>
void fill_const_cast_for(auto& method) {
  fill_const_cast_for(type_list<CLASSES...>{}, method);
}
void fill_cast_for(auto classes, auto& method) {
  fill_with_overloads(classes, method, cast_implementation{});
}
template <typename... CLASSES>
void fill_cast_for(auto& method) {
  fill_cast_for(type_list<CLASSES...>{}, method);
}
template <typename TO>
auto cast_to(const auto& cast, is_virtual_void auto const& from) {
  if (auto void_ = cast(from, typeid_of<std::remove_const_t<TO>>()))
    return static_cast<TO*>(void_);
  return static_cast<TO*>(nullptr);
}

}  // namespace virtual_void::typeid_cast