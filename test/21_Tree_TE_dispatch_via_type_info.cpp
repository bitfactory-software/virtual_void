﻿// virtual_void variant of this yomm2 example via c++RTTI
// https://github.com/jll63/yomm2/blob/master/examples/accept_no_visitors.cpp

#include <iostream>
#include <memory>
#include <string>

#include "../../include/virtual_void/utillities/unnamed__.h"
#include "../../include/virtual_void/erased/dispatch/open_method/typeid/open_method.h"
#include "include/catch.hpp"

using std::cout;
using std::string;

namespace {

struct Node {
  virtual ~Node() = default;  // generates c++ vtable + type_info
};

using shared_const_node = std::shared_ptr<const Node>;

struct Plus : Node {
  Plus(shared_const_node left, shared_const_node right)
      : left(left), right(right) {}

  shared_const_node left, right;
};

struct Times : Node {
  Times(shared_const_node left, shared_const_node right)
      : left(left), right(right) {}

  shared_const_node left, right;
};

struct Integer : Node {
  explicit Integer(int value) : value(value) {}
  int value;
};

// =============================================================================
// add behavior to existing classes, without changing them

virtual_void::typeid_::domain tree_open_methods;

//+++ no special meta data needed. the dispatch information comes from the
//typeid() via the c++ vtable
//
// namespace virtual_void::class_hierarchy
//{
//    template<> struct class_< Node > : base {};
//    template<> struct class_< Plus > : bases< Node >{};
//    template<> struct class_< Times > : bases< Node >{};
//    template<> struct class_< Integer > : bases< Node >{};
//
//	auto __ = declare_classes< Node, Plus, Times, Integer >( tree_domain );
//}
//---

// -----------------------------------------------------------------------------
// evaluate

auto value =
    virtual_void::typeid_::open_method<int(const void*)>{tree_open_methods};

auto __ = value.define<Plus>(
    [](auto expr) { return value(expr->left) + value(expr->right); });

auto __ = value.define<Times>(
    [](auto expr) { return value(expr->left) * value(expr->right); });

auto __ = value.define<Integer>([](auto expr) { return expr->value; });

// -----------------------------------------------------------------------------
// render as Forth

auto as_forth =
    virtual_void::typeid_::open_method<string(const void*)>{tree_open_methods};

auto __ = as_forth.define<Plus>([](auto expr) {
  return as_forth(expr->left) + " " + as_forth(expr->right) + " +";
});

auto __ = as_forth.define<Times>([](auto expr) {
  return as_forth(expr->left) + " " + as_forth(expr->right) + " *";
});

auto __ = as_forth.define<Integer>(
    [](auto expr) { return std::to_string(expr->value); });

// -----------------------------------------------------------------------------
// render as Lisp

auto as_lisp =
    virtual_void::typeid_::open_method<string(const void*)>{tree_open_methods};

auto __ = as_lisp.define<Plus>([](auto expr) {
  return "(plus " + as_lisp(expr->left) + " " + as_lisp(expr->right) + ")";
});

auto __ = as_lisp.define<Times>([](auto expr) {
  return "(times " + as_lisp(expr->left) + " " + as_lisp(expr->right) + ")";
});

auto __ = as_lisp.define<Integer>(
    [](auto expr) { return std::to_string(expr->value); });

}  // namespace

// -----------------------------------------------------------------------------

TEST_CASE("21_Tree_TE_dispatch_via_type_info") {
  // build_m_tables( tree_domain ); no v_tabls, dispatch via
  // typeindex(type_info)->"define" function
  virtual_void::typeid_::seal_for_runtime(tree_open_methods);

  using std::make_shared;

  auto expr = make_shared<Times>(
      make_shared<Integer>(2),
      make_shared<Plus>(make_shared<Integer>(3), make_shared<Integer>(4)));

  value(expr) == 14;
  REQUIRE(value(expr) == 14);
  std::stringstream out;
  out << as_forth(expr) << " = " << as_lisp(expr) << " = " << value(expr);
  REQUIRE(out.str() == "2 3 4 + * = (times 2 (plus 3 4)) = 14");
  std::cout << out.str() << "\n";
#ifndef _DEBUG
  BENCHMARK("21_Tree_TE_dispatch_via_type_info value") { return value(expr); };
  BENCHMARK("21_Tree_TE_dispatch_via_type_info as_lisp") {
    return as_lisp(expr);
  };
#endif  // !_DEBUG
}
