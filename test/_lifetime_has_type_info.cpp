#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "class_hierarchy_test_hierarchy.hpp"

#include <catch.hpp>

#include <virtual_void/data/has_type_info/observer.hpp>
#include <virtual_void/data/has_type_info/shared_const.hpp>
#include <virtual_void/data/has_type_info/unique.hpp>
#include <virtual_void/data/has_type_info/value.hpp>
#include <virtual_void/data/has_type_info/shared_const_ptr.hpp>
#include <virtual_void/data/has_type_info/unique_ptr.hpp>

#include "a.hpp"

using namespace Catch::Matchers;

using namespace virtual_void;
using namespace ::virtual_void::data::has_type_info;
using namespace TestDomain;

namespace {

TEST_CASE("has_type_info/lifetime/observer") {
  std::string s{"hallo"};
  auto mo = erased<mutable_observer>(s);
    mutable_observer o1;
 REQUIRE(!o1);
  o1 = mo;
  REQUIRE(o1);
  REQUIRE(get_data(mo) == &s);
  REQUIRE(get_data(o1) == &s);
  REQUIRE(*static_cast<std::string const*>(get_data(mo)) == "hallo");
  REQUIRE(get_meta(mo)->type_info() == &typeid(std::string));
  REQUIRE(*static_cast<std::string const*>(get_data(mo)) == "hallo");
  REQUIRE(*unerase_cast<const std::string>(mo) == "hallo");
  static_assert(
      std::same_as<typed_mutable_observer<std::string>::value_t, std::string>);
  static_assert(std::same_as<typed_const_observer<std::string const>::value_t,
                             std::string const>);
  auto co = erased<const_observer>(s);
  REQUIRE(*unerase_cast<const std::string>(co) == "hallo");
  {
    REQUIRE(*unerase_cast<const std::string>(mo) == "hallo");
    auto tmo = as<std::string>(std::move(mo));
    REQUIRE(*tmo == "hallo");
    *tmo = "world";
    REQUIRE(s == "world");
    REQUIRE(*unerase_cast<const std::string>(co) == "world");
  }
  {
    mo = erased<mutable_observer>(s);
    REQUIRE(*unerase_cast<const std::string>(mo) == "world");
    *unerase_cast<std::string>(mo) = "hallo";
    REQUIRE(s == "hallo");
    auto tmo = as<std::string>(mo);
    REQUIRE(*unerase_cast<std::string>(mo) == "hallo");
    REQUIRE(has_data(mo));
    REQUIRE(*tmo == "hallo");
    *tmo = "world";
    REQUIRE(s == "world");
    REQUIRE(*unerase_cast<const std::string>(co) == "world");
    REQUIRE(*unerase_cast<std::string>(mo) == "world");
  }
  // auto tmo2 = typed_observer< std::string * >{ co }; // shall not
  // compile
}

TEST_CASE("has_type_info/lifetime/shared_const") {
  auto d = erased_in_place<shared_const, D>("shared hallo");
  auto d1 = as<D const>(d);
  REQUIRE(d1->data == "shared hallo");
  REQUIRE(get_meta(d1)->type_info() == &typeid(D));
  static_assert(std::derived_from<D, A1>);
  typed_shared_const<A1> a0{*d1};
  auto a1 = as<A1 const>(d1);
  typed_shared_const<A1> a2{A1{"a2->OK"}};
  typed_shared_const<A1> a3{std::in_place, "a3 in_place->OK"};
  A1 a1_pur{"a1_pur"};
  typed_shared_const<A1> a4{a1_pur};
  auto& a1r = *a2;
  auto s1 { a1r.data };
  auto s { a2->data };
  REQUIRE(a2->data == "a2->OK");
  REQUIRE(a3->data == "a3 in_place->OK");
  REQUIRE(a4->data == "a1_pur");
}

TEST_CASE("has_type_info/lifetime/unique") {
  auto c1 = erased_in_place<unique, C>("unique c1");
  REQUIRE(unerase_cast<C>(c1)->data == "unique c1");
  auto c2 = typed_unique<C>(std::in_place, "unique c2");
  REQUIRE(c2->data == "unique c2");
  auto c3 = typed_unique<C>(C{"unique c3"});
  REQUIRE(c3->data == "unique c3");
  auto c4 { std::move(c3) };
  REQUIRE(c4->data == "unique c3");

  unique d1 = erased_in_place<unique, D>("unique hallo");
  static_assert(is_virtual_void<unique>);
  unique x{std::move(d1)};
  auto d = as<D>(std::move(x));
  REQUIRE(d->data == "unique hallo");
  virtual_typed<D, unique> d2 = as<D>(unique_nullptr());
  REQUIRE(!d2);
  d2 = std::move(d);
  REQUIRE(d2->data == "unique hallo");
  REQUIRE(d2);
}

TEST_CASE("has_type_info/lifetime/value") {
  {
    auto u1 = erased<value>(1);
    REQUIRE(*unerase_cast<int>(u1) == 1);
  }
  {
    auto u1 = erased<value>(A{"hallo"});
    auto& u1cr = u1;
    auto a = unerase_cast<A>(u1);
    REQUIRE(a->s == "hallo");
    auto a1 = unerase_cast<A>(u1cr);
    REQUIRE(a1->s == "hallo");
    REQUIRE(unerase_cast<A>(u1)->s == "hallo");
    auto u2 { u1 };
    REQUIRE(unerase_cast<A>(u1)->s.data() !=
            unerase_cast<A>(u2)->s.data());
    REQUIRE(unerase_cast<A>(u2)->s == "hallo");
    unerase_cast<A>(u2)->s = "world";
    REQUIRE(unerase_cast<A>(u1)->s == "hallo");
    REQUIRE(unerase_cast<A>(u2)->s == "world");
  }
  {
    auto v1 = typed_value<std::string>("hallo");
    static_assert(std::same_as<std::decay_t<decltype(*v1)>, std::string>);
    REQUIRE(*v1 == std::string{"hallo"});
    auto v2 { std::move(v1) };
#pragma warning( push )
#pragma warning( disable : 26800)
    REQUIRE(!has_data(v1));
#pragma warning( pop )
    REQUIRE(*v2 == "hallo");
    v1 = std::move(v2);
#pragma warning( push )
#pragma warning( disable : 26800)
    REQUIRE(!has_data(v2));
#pragma warning( pop )
    REQUIRE(*v1 == "hallo");
  }
  {
    auto t1 = typed_value<int>(1);
    *t1 = 2;
    REQUIRE(*t1 == 2);
    auto e1 { t1 };
    REQUIRE(has_data(t1));  // !moved
    REQUIRE(get_data(e1));
    t1 = as<int>(std::move(e1));
#pragma warning( push )
#pragma warning( disable : 26800)
    REQUIRE(!has_data(e1));  // !moved
#pragma warning( pop )
    REQUIRE(*t1 == 2);
  }
  {
    auto t1 = typed_value<std::string>("hallo");
    REQUIRE(*t1 == "hallo");
  }
  {
    std::string a = "hallo";
    auto t1 = erased<value>(a);
    REQUIRE(*unerase_cast<std::string>(t1) == "hallo");
  }
  {
    struct x_t {
      std::string s_;
    };
    x_t a{"hallo"};
    auto t1 = erased<value>(a);
    REQUIRE(unerase_cast<x_t>(t1)->s_ == "hallo");
  }
}
TEST_CASE("has_type_info/shared_const_ptr") {
  {
    auto ptr = std::make_shared<A>("hallo");
    REQUIRE(ptr->s == "hallo");
    shared_const_ptr sp1 =
        virtual_void_trait<shared_const_ptr>::construct_from(ptr);
    auto u1 = erased<shared_const_ptr>(ptr);
    A const* a = unerase_cast<A>(u1);
    REQUIRE(unerase_cast<A>(u1)->s == "hallo");
  }
}
TEST_CASE("has_type_info/unique_ptr") {
  {
    auto ptr = std::make_unique<A>("hallo");
    REQUIRE(ptr->s == "hallo");
    unique_ptr up1 =
        virtual_void_trait<unique_ptr>::construct_from(std::move(ptr));
  }
  {
    auto ptr = std::make_unique<A>("hallo");
    REQUIRE(ptr->s == "hallo");
    auto u1 = erased<unique_ptr>(std::move(ptr));
    A const* a = unerase_cast<A>(u1);
    REQUIRE_THROWS_AS(unerase_cast<std::string>(u1), type_mismatch_error);
    REQUIRE(unerase_cast<A>(u1)->s == "hallo");
    auto typed = as<A>(std::move(u1));
    REQUIRE(typed->s == "hallo");
  }
}
}  // namespace
