#pragma once

#include <typeindex>

#include <virtual_void/data/has_type_info/observer.hpp>
#include <virtual_void/utillities/ensure_function_ptr.hpp>
#include <virtual_void/utillities/overload.hpp>
#include <virtual_void/open_method/via_type_info/declaration_base.hpp>
#include <virtual_void/virtual_void.hpp>

namespace virtual_void::open_method::via_type_info {

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
  using param_t = data::has_type_info::observer<dispatch_t>;
  using erased_function_t = typename translate_erased_function< R, ARGS...>::type;

 public:
  using declaration_base::declaration_base;
  template <typename CLASS, typename FUNCTION>
  auto define(FUNCTION f) {
    if (dispatch_target_index_) throw error("Already sealed for runtime.");
    auto fp = ensure_function_ptr<CLASS, class_param_t, R, ARGS...>(f);
    return define_erased(typeid_of<CLASS>(), fp);
  }
  template <typename... OTHER_ARGS>
  R operator()(const std::type_info& type_info, dispatch_t dispatched,
               OTHER_ARGS&&... args) const {
    auto f = lookup<erased_function_t>(type_info);
    return f(dispatched, std::forward<OTHER_ARGS>(args)...);
  }
  template <is_virtual_void VIRTUAL_VOID, typename... OTHER_ARGS>
  R operator()(const VIRTUAL_VOID& param, OTHER_ARGS&&... args) const
//    requires const_correct_for_virtual_void<dispatch_t, VIRTUAL_VOID>
  {
    return (*this)(*get_meta(param)->type_info(), get_data(param),
                   std::forward<OTHER_ARGS>(args)...);
  }
  template <typename CLASS, typename... OTHER_ARGS>
  R operator()(CLASS* param, OTHER_ARGS&&... args) const {
    return (*this)(typeid(*param), static_cast<dispatch_t>(param),
                   std::forward<OTHER_ARGS>(args)...);
  }
  template <typename CLASS, typename... OTHER_ARGS>
  R operator()(const std::shared_ptr<CLASS>& param,
               OTHER_ARGS&&... args) const {
    return (*this)(param.get(), std::forward<OTHER_ARGS>(args)...);
  }
};

}  // namespace virtual_void::open_method::via_type_info