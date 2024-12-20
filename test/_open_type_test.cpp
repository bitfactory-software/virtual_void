#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <catch.hpp>

using namespace Catch::Matchers;

#include <virtual_void/open_object/members.hpp>

using namespace virtual_void;
using namespace virtual_void::open_object;

struct test_object : members<test_object> {};  // name your open_object

inline const member<test_object, std::string> test_member;  // define your member

TEST_CASE("open object 1") {
  REQUIRE(open_object::type_member_count_of<test_object> == 1);
  test_object a_test_object;
  auto value = a_test_object.get(test_member);
  REQUIRE(!value);
  a_test_object.set(test_member, "hello world");
  auto r = a_test_object.get(test_member);
  REQUIRE(r);
  std::cout << typeid(*r).name() << "\n";
  REQUIRE(*r == "hello world");
  static_assert(std::same_as<decltype(r), std::string*>);
  REQUIRE((*a_test_object.get(test_member)) == "hello world");
  {
    test_object const& const_test_object = a_test_object;
    auto r = const_test_object.get(test_member);
    REQUIRE(r);
    std::cout << typeid(*r).name() << "\n";
    REQUIRE(*r == "hello world");
    static_assert(std::same_as<decltype(r), std::string const*>);
    REQUIRE((*const_test_object.get(test_member)) == "hello world");
  }
  *a_test_object.get(test_member) = "hello programm";
  REQUIRE((*a_test_object.get(test_member)) == "hello programm");
}
TEST_CASE("open object 2") {
  test_object a_test_object;
  auto r = a_test_object.get(test_member);
  REQUIRE(!r);
  a_test_object[test_member] = "hello world";
  REQUIRE(a_test_object[test_member] == "hello world");
}
TEST_CASE("open object 3") {
  test_object a_test_object;
  auto r = a_test_object.get(test_member);
  REQUIRE(!r);
  REQUIRE(a_test_object[test_member] == "");
}
TEST_CASE("test fails for test test") {
  REQUIRE(true); // for now
}
