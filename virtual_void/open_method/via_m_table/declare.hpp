#pragma once

#include <set>
#include <typeindex>
#include <unordered_map>

#include <virtual_void/utillities/VV_EXPORT.hpp>
#include <virtual_void/meta/class.hpp>
#include <virtual_void/data/has_m_table/m_table.hpp>
#include <virtual_void/utillities/ensure_function_ptr.hpp>
#include <virtual_void/utillities/overload.hpp>
#include <virtual_void/utillities/type_list.hpp>
#include <virtual_void/virtual_void.hpp>
#include <virtual_void/open_method/algorithm.hpp>
#include <virtual_void/open_method/default_target.hpp>
#include <virtual_void/open_method/domain.hpp>

namespace virtual_void::open_method::via_m_table {

class declaration_base;
using open_methods = std::vector<declaration_base*>;
using m_table_map =
    std::unordered_map<std::type_info const*, data::has_m_table::m_table_t*>;

struct domain : open_method::domain<declaration_base> {
  m_table_map m_table_map;
  ~domain() {
    for (auto entry : m_table_map) entry.second->clear();
  }
};

class declaration_base : public open_method::default_target<> {
  const int m_table_index_ = -1;
  domain& domain_;

 public:
  using dispatch_target_t = declaration_base::dispatch_target_t;
  struct definition {};
  int m_table_index() const { return m_table_index_; }

  explicit declaration_base(domain& domain)
      : m_table_index_((int)domain.open_methods.size()), domain_(domain) {
    domain.open_methods.push_back(this);
  }
  auto define_erased(data::has_m_table::m_table_t* m_table, auto f) {
    auto t = reinterpret_cast<dispatch_target_t>(f);
    m_table->set_method(m_table_index(), t);
    return definition{};
  }
  auto define_erased(const std::type_info& type_info, auto f) {
    auto m_table = domain_.m_table_map[&type_info];
    return define_erased(m_table, f);
  }
  template <typename CLASS>
  auto define_erased(auto f) {
    auto m_table = data::has_m_table::m_table_of<CLASS>();
    domain_.m_table_map[&typeid_of<CLASS>()] = m_table;
    return define_erased(m_table, f);
  }
  template <typename CLASS>
  dispatch_target_t is_defined() const {
    return data::has_m_table::m_table_of<CLASS>()->find(m_table_index());
  }
  dispatch_target_t is_defined(const std::type_info& type_info) const {
    if (auto found = domain_.m_table_map.find(&type_info);
        found != domain_.m_table_map.end())
      return found->second->find(m_table_index());
    return {};
  }
};

using m_table_t = data::has_m_table::m_table_t;

template <typename DISPATCH, typename VV_VOID>
concept is_m_table_dispachable_virtual_void = requires(const DISPATCH& void_) {
  { void_.data() } -> std::convertible_to<VV_VOID>;
  { void_.m_table() } -> std::convertible_to<const m_table_t*>;
};

template <typename R, typename... ARGS>
class declare;

template <typename R, typename... ARGS>
class declare<R(ARGS...)> : public declaration_base {
  static_assert(is_constness<first_t<ARGS...>>);

 public:
  using CONSTNESS = typename first_t<ARGS...>;
  using dispatch_t = void_t<CONSTNESS>;
  template <typename CLASS>
  using class_param_t = self_pointer<dispatch_t>::template type<CLASS>;
  using pair_t = std::pair<const data::has_m_table::m_table_t*, dispatch_t>;
  using erased_function_t =
      typename translate_erased_function<R, ARGS...>::type;
  using declaration_base::declaration_base;

  template <typename CLASS, typename FUNCTION>
  auto define(FUNCTION f) {
    auto fp = ensure_function_ptr<CLASS, class_param_t, R, ARGS...>(f);
    return declaration_base::define_erased<CLASS>(fp);
  }
  template <typename... OTHER_ARGS>
  R operator()(data::has_m_table::m_table_t const& m_table, dispatch_t data,
               OTHER_ARGS&&... args) const {
    auto erased_function = reinterpret_cast<erased_function_t>(
        m_table.at(m_table_index(), get_default()));
    return (erased_function)(data, std::forward<OTHER_ARGS>(args)...);
  }
  template <is_virtual_void DATA, typename... OTHER_ARGS>
  R operator()(const DATA& virtual_void_, OTHER_ARGS&&... args) const {
    return (*this)(*get_meta(virtual_void_)->get_m_table(),
                   get_data(virtual_void_), std::forward<OTHER_ARGS>(args)...);
  }
  template <typename... OTHER_ARGS>
  R operator()(const pair_t& param, OTHER_ARGS&&... args) const {
    return (*this)(*param.first, param.second,
                   std::forward<OTHER_ARGS>(args)...);
  }
  template <typename POINTER, typename... OTHER_ARGS>
  R operator()(const POINTER& pointer, OTHER_ARGS&&... args) const
    requires is_m_table_dispachable_virtual_void<POINTER, dispatch_t>
  {
    return (*this)(*pointer.m_table(), pointer.data(),
                   std::forward<OTHER_ARGS>(args)...);
  }
  template <typename POINTER, typename... OTHER_ARGS>
  R call(const POINTER& pointer, OTHER_ARGS&&... args) const
    requires is_m_table_dispachable_virtual_void<POINTER, dispatch_t>
  {
    return (*this)(pointer, std::forward<OTHER_ARGS>(args)...);
  }
};

template <typename CLASSES>
constexpr nullptr_t declare_classes(m_table_map& registry) {
  meta::visit_classes<CLASSES, true>(
      overload{[&]<typename C> {
                 registry[&typeid_of<C>()] = data::has_m_table::m_table_of<C>();
               },
               [&]<typename C, typename B> {}});
  return {};
}

template <typename CLASSES>
constexpr nullptr_t declare_classes(CLASSES classes, domain& domain) {
  declare_classes<CLASSES>(domain.m_table_map);
  return ::virtual_void::open_method::declare_classes(classes, domain);
}

template <typename... CLASSES>
constexpr nullptr_t declare_classes(domain& domain) {
  return declare_classes(::virtual_void::type_list<CLASSES...>{}, domain);
}

}  // namespace virtual_void::open_method::via_m_table