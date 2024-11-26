#include <cmath>
#include <iostream>
#include <string>
#include <vector>

 #include "virtual_void/interface/call_operator.hpp"
 #include "virtual_void/interface/declare_macro.hpp"
 #include "virtual_void/data/has_no_meta/observer.hpp"
 #include "virtual_void/data/has_no_meta/unique.hpp"
 #include "virtual_void/data/has_m_table/shared_const.hpp"

#include <catch.hpp>

using namespace Catch::Matchers;

using namespace virtual_void;

namespace {
template <typename T>
struct test_trait;
template <>
struct test_trait<const void*> {
  static const bool is_const = true;
};
template <>
struct test_trait<void*> {
  static const bool is_const = false;
};
template <typename VOID>
struct test_interface {
  VOID member = nullptr;
  std::string f(int)
    requires(!test_trait<VOID>::is_const)
  {
    return "mutable";
  }
  std::string f(int) const { return "const"; }
};
TEST_CASE("_interface_const_correct prototyping") {
  using namespace virtual_void;
  static_assert(!test_trait<void*>::is_const);
  static_assert(
      virtual_void_trait<data::has_no_meta::const_observer>::is_const);
  test_interface<void const*> i1;
  REQUIRE(i1.f(1) == "const");

  test_interface<void const*> const i2;
  REQUIRE(i2.f(1) == "const");

  test_interface<void*> i3;
  REQUIRE(i3.f(1) == "mutable");

  test_interface<void*> const i4;
  REQUIRE(i4.f(1) == "const");
}
}  // namespace

namespace {
struct functor {
  std::string text = "hallo";
  std::string operator()() const { return text; }
  void operator()(const std::string& t) { text = t; }
};

TEST_CASE("_interface_const_correct void (const) *") {
  using namespace virtual_void;

  using const_function =
      interface::call_operator<data::has_no_meta::const_observer,
                               std::string()>;
  using mutating_function =
      interface::call_operator<data::has_no_meta::mutable_observer,
                               void(std::string), mutable_>;

  {
    functor function_object;
    const_function cf = function_object;
    mutating_function mf = function_object;
    REQUIRE(cf() == "hallo");
    mf("world");
    REQUIRE(cf() == "world");
  }

  {
    functor function_object;
    const_function const cf = function_object;
    mutating_function const mf = function_object;
    REQUIRE(cf() == "hallo");
    mf("world");
    REQUIRE(cf() == "world");
  }

  {
    functor const const_function_object;
    const_function cf = const_function_object;
    REQUIRE(cf() == "hallo");
  }

  {
    functor const function_object;
    const_function const cf = function_object;
    REQUIRE(cf() == "hallo");
  }
}

TEST_CASE("_interface_const_correct virtual_void::shared_const") {
  using const_function =
      interface::call_operator<data::has_m_table::shared_const, std::string()>;
  using mutating_function =
      interface::call_operator<data::has_m_table::shared_const, void(std::string), mutable_>;

  {
    functor function_object;
    const_function cf = function_object;
    static_assert(
        !std::is_assignable_v<mutating_function,
                              const_function>);  // <- may not compile!
    REQUIRE(cf() == "hallo");
  }

  {
    functor function_object;
    const_function const cf = function_object;
    static_assert(
        !std::is_assignable_v<mutating_function,
                              const_function const>);  // <- may not compile!
    REQUIRE(cf() == "hallo");
  }

  {
    functor const const_function_object;
    const_function cf = const_function_object;
    static_assert(!std::is_assignable_v<mutating_function,
                                        functor const>);  // <- may not compile!
    REQUIRE(cf() == "hallo");
  }

  {
    functor const function_object;
    const_function const cf = function_object;
    static_assert(!std::is_assignable_v<mutating_function const,
                                        functor const>);  // <- may not compile!
    REQUIRE(cf() == "hallo");
  }
}
}  // namespace
