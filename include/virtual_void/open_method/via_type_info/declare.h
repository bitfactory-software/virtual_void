#pragma once

#include <typeindex>

#include "../../utillities/ensure_function_ptr.h"
#include "../../utillities/overload.h"
#include "../../data/has_type_info/observer.h"
#include "declaration_base.h"

namespace virtual_void::open_method::via_type_info {

template <typename R, typename... ARGS>
class declare;
template <typename R, typename... ARGS>
class declare<R(ARGS...)> : public declaration_base {
  static_assert(std::same_as<first_t<ARGS...>, void*> ||
                std::same_as<first_t<ARGS...>, const void*>);

 public:
  using dispatch_t = typename first_t<ARGS...>;
  template <typename CLASS>
  using class_param_t = self_pointer<dispatch_t>::template type<CLASS>;
  using param_t = data::has_type_info::observer<dispatch_t>;
  using erased_function_t = R (*)(ARGS...);

 public:
  using declaration_base::declaration_base;
  template <typename CLASS, typename FUNCTION>
  auto define(FUNCTION f) {
    if (dispatch_target_index_) throw error("Already sealed for runtime.");
    auto fp = ensure_function_ptr<CLASS, class_param_t, R, ARGS...>(f);
    return define_erased(typeid(CLASS), fp);
  }
  template <typename... OTHER_ARGS>
  R operator()(const std::type_info& type_info, dispatch_t dispatched,
               OTHER_ARGS&&... args) const {
    auto f = lookup<erased_function_t>(type_info);
    return f(dispatched, std::forward<OTHER_ARGS>(args)...);
  }
  template <typename... OTHER_ARGS>
  R operator()(const param_t& param, OTHER_ARGS&&... args) const {
    return (*this)(*param.meta()->type_info(), get_data(param),
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