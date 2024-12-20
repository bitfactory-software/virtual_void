﻿// virtual_void variant of this yomm2 example via c++RTTI
// https://github.com/jll63/yomm2/blob/master/examples/accept_no_visitors.cpp

#include <iostream>
#include <memory>
#include <string>

#include <catch.hpp>

#include <virtual_void/data/has_no_meta/shared_const.hpp>
#include <virtual_void/interface/base.hpp>
#include <virtual_void/data/has_no_meta/shared_const_ptr.hpp>
#include <virtual_void/data/make_shared_const_decorated_data.hpp>

#include <virtual_void/interface/declare_macro.hpp>

using std::cout;
using std::string;

using namespace virtual_void;
using namespace virtual_void::data::has_no_meta;

namespace {


VV_INTERFACE(node_i, (VV_CONST_METHOD(int, value),
                          VV_CONST_METHOD(string, as_forth),
                          VV_CONST_METHOD(string, as_lisp)))

// alternative: using node = node_i<shared_const>; less code, but intrusive
// lifetime management
using node = node_i<shared_const_ptr>;

struct Plus {
  Plus(node left, node right) : left(left), right(right) {}
  int value() const { return left.value() + right.value(); }
  string as_forth() const {
    return left.as_forth() + " " + right.as_forth() + " +";
  }
  string as_lisp() const {
    return "(plus " + left.as_lisp() + " " + right.as_lisp() + ")";
  }

  node left, right;
};

struct Times {
  Times(node left, node right) : left(left), right(right) {}
  int value() const { return left.value() * right.value(); }
  string as_forth() const {
    return left.as_forth() + " " + right.as_forth() + " *";
  }
  string as_lisp() const {
    return "(times " + left.as_lisp() + " " + right.as_lisp() + ")";
  }

  node left, right;
};

struct Integer {
  explicit Integer(int value) : int_(value) {}
  int value() const { return int_; }
  string as_forth() const { return std::to_string(int_); }
  string as_lisp() const { return std::to_string(int_); }

  int int_;
};

//alternative for node = node_i<shared_const>:
// template <typename NODE, typename... ARGS>
// auto make_node(ARGS&&... args) {
//  return node{NODE(std::forward<ARGS>(args)...)};
//}

template <typename NODE, typename... ARGS>
auto make_node(ARGS&&... args) {
  return node{std::make_shared<NODE>(std::forward<ARGS>(args)...)};
}

}  // namespace

TEST_CASE("21_Tree_TE_dynamic_interface") {
  using namespace virtual_void;

  auto expr = node(make_node<Times>(
      make_node<Integer>(2),
      make_node<Plus>(make_node<Integer>(3), make_node<Integer>(4))));

  REQUIRE(expr.value() == 14);
  std::stringstream out;
  out << expr.as_forth() << " = " << expr.as_lisp() << " = " << expr.value();
  REQUIRE(out.str() == "2 3 4 + * = (times 2 (plus 3 4)) = 14");
  std::cout << out.str() << "\n";
#ifndef _DEBUG
  BENCHMARK("21_Tree_TE_dynamic_interface value") { return expr.value(); };
  BENCHMARK("21_Tree_TE_dynamic_interface as_lisp") { return expr.as_lisp(); };
#endif  // !_DEBUG
}
