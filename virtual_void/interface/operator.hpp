#pragma once

#undef interface

#include <virtual_void/interface/base.hpp>
#include <virtual_void/virtual_void.hpp>

namespace virtual_void::interface {

template <typename TARGET, typename BASE_V_TABLE, is_constness CONSTNESS,
          typename RET, typename... ARGS>
struct operator_v_table : BASE_V_TABLE {
  using v_table_base_t = BASE_V_TABLE;
  using v_table_t = operator_v_table;
  static bool static_is_derived_from(const std::type_info& from) {
    return typeid(v_table_t) == from
               ? true
               : BASE_V_TABLE::static_is_derived_from(from);
  }
  RET (*op)(virtual_void::void_t<CONSTNESS>, ARGS&&...);
  template <is_uneraser UNERASER>
  operator_v_table(UNERASER u) : BASE_V_TABLE(u) {
    op = [](virtual_void::void_t<CONSTNESS> _vp, ARGS&&... args) -> RET {
      return TARGET{}(UNERASER{}(_vp), std::forward<ARGS>(args)...);
    };
    set_is_derived_from<v_table_t>(this);
  }
};

template <is_uneraser UNERASER, typename TARGET, typename BASE_V_TABLE,
          is_constness CONSTNESS, typename RET, typename... ARGS>
operator_v_table<TARGET, BASE_V_TABLE, CONSTNESS, RET, ARGS...>*
implemented_operator_v_table() {
  static operator_v_table<TARGET, BASE_V_TABLE, CONSTNESS, RET, ARGS...>
      v_table{UNERASER{}};
  return &v_table;
}

template <typename TARGET, is_virtual_void VIRTUAL_VOID,
          template <typename> typename BASE, is_constness CONSTNESS,
          typename RET, typename... ARGS>
struct operator_;
template <typename TARGET, is_virtual_void VIRTUAL_VOID,
          template <typename> typename BASE, is_constness CONSTNESS,
          typename RET, typename... ARGS>
struct operator_<TARGET, VIRTUAL_VOID, BASE, CONSTNESS, RET(ARGS...)>
    : BASE<VIRTUAL_VOID> {
 public:
  using virtual_void_t = VIRTUAL_VOID;
  using void_t = typename virtual_void_trait<VIRTUAL_VOID>::void_t;
  using base_t = BASE<VIRTUAL_VOID>;
  using v_table_base_t = base_t::v_table_t;
  using v_table_t =
      operator_v_table<TARGET, v_table_base_t, CONSTNESS, RET, ARGS...>;
  using base_t::operator();

 protected:
  using base_t::v_table_;
  using base_t::virtual_void_;

 public:
  operator_(virtual_void_t virtual_void, v_table_t* v_table)
      : base_t(std::move(virtual_void), v_table) {}
  template <typename CONSTRUCTED_WITH>
  operator_(CONSTRUCTED_WITH&& v)
    requires constructibile_for<CONSTRUCTED_WITH, VIRTUAL_VOID>
      : base_t(std::forward<CONSTRUCTED_WITH>(v)) {
    v_table_ =
        implemented_operator_v_table<uneraser<VIRTUAL_VOID, CONSTRUCTED_WITH>,
                                     TARGET, v_table_base_t, CONSTNESS, RET,
                                     ARGS...>();
  }
  template <typename OTHER>
  operator_(const OTHER& other)
    requires(std::derived_from<OTHER, base_t>)
      : base_t(other) {}
  // RET operator()(ARGS&&... args) const
  //   requires(const_correct_for_virtual_void<
  //            virtual_void::void_t<CONSTNESS>, virtual_void_t>)
  //{
  //   return static_cast<v_table_t*>(v_table_)->call_op(
  //       get_data(base_t::virtual_void_), std::forward<ARGS>(args)...);
  // }
  RET invoke(ARGS&&... args) const
    requires(const_correct_for_virtual_void<virtual_void::void_t<CONSTNESS>,
                                            virtual_void_t>)
  {
    return static_cast<v_table_t*>(v_table_)->op(
        get_data(base_t::virtual_void_), std::forward<ARGS>(args)...);
  }
  operator_(const operator_&) = default;
  operator_(operator_&) = default;
  operator_(operator_&&) = default;

 protected:
  operator_() = default;
};

};  // namespace virtual_void::interface
