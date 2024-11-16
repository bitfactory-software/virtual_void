﻿#include <iostream>

#include "include/catch.hpp"

import virtual_void;
// #include "virtual_void/data/has_type_info/observer.h"
// #include "virtual_void/data/has_type_info/shared_const.h"
// #include "virtual_void/interface/base.h"
// #include "virtual_void/interface/registry.h"
#include "virtual_void/interface/declare_macro.h"

using namespace virtual_void;
using namespace virtual_void::interface;
using namespace virtual_void::data::has_type_info;

namespace {
struct X {
  double d_;
  std::string to_string() const { return std::to_string(d_); }
  double get_value() const { return d_; }
  void set_value(double d) { d_ = d; }
};

// ERASED_INTERFACE(to_string_i, (INTERFACE_CONST_METHOD(std::string,
// to_string))) ERASED_INTERFACE(get_value_i, (INTERFACE_CONST_METHOD(double,
// get_value))) ERASED_INTERFACE(set_value_i, (INTERFACE_METHOD(void, set_value,
// double)))

ERASED_INTERFACE(to_string_i, (INTERFACE_CONST_METHOD(std::string, to_string)))
ERASED_INTERFACE(get_value_i, (INTERFACE_CONST_METHOD(double, get_value)))
ERASED_INTERFACE(set_value_i, (INTERFACE_METHOD(void, set_value, double)))

}  // namespace

TEST_CASE("prototype") {
  enable_interface_copy<get_value_i, X, const_observer>();
  enable_interface_copy<get_value_i, X, mutable_observer>();
  enable_interface_copy<get_value_i, X, shared_const>();
  enable_interface_copy<get_value_i, X, unique>();
  enable_interface_copy<get_value_i, X, unique, shared_const>();
  seal_for_runtime(query_interface_domain);

  {
    X x{3.14};
    to_string_i<const_observer> to_string_i_co{x};
    REQUIRE(to_string_i_co.to_string() == "3.140000");
    auto base = copy<X, const_observer>.construct<get_value_i<const_observer>>(
        get_virtual_void(to_string_i_co));
    auto i = interface::static_v_table_cast<get_value_i<const_observer>>(base);
    REQUIRE(i.get_value() == 3.14);
    std::cout << "prototype: " << i.get_value() << "\n";

    get_value_i<const_observer> i1 =
        query_interface<get_value_i<const_observer>>(to_string_i_co);
    REQUIRE(i1.get_value() == 3.14);
    std::cout << "prototype i1: " << i1.get_value() << "\n";
    REQUIRE(get_data(get_virtual_void(i1)) ==
            get_data(get_virtual_void(to_string_i_co)));
  }
  {
    X x{3.14};
    to_string_i<shared_const> i0{x};
    REQUIRE(i0.to_string() == "3.140000");
    std::cout << "prototype shared_const i0: " << i0.to_string() << "\n";

    get_value_i<shared_const> i1 =
        query_interface<get_value_i<shared_const>>(i0);
    REQUIRE(i1.get_value() == 3.14);
    std::cout << "prototype shared_const i1: " << i1.get_value() << "\n";
    REQUIRE(get_data(get_virtual_void(i1)) == get_data(get_virtual_void(i0)));

    get_value_i<unique> iu1 = query_interface<get_value_i<unique>>(i0);
    REQUIRE(iu1.get_value() == 3.14);
    std::cout << "prototype shared_const iu1: " << iu1.get_value() << "\n";
    REQUIRE(get_data(get_virtual_void(iu1)) != get_data(get_virtual_void(i0)));
  }
  {
    X x{3.14};
    to_string_i<unique> i0{x};
    REQUIRE(i0.to_string() == "3.140000");
    std::cout << "prototype unique i0: " << i0.to_string() << "\n";


        //      get_value_i<unique> i1 =
    base<unique> i1b = query_interface<get_value_i<unique>>(i0);
    auto i1 = std::move(static_v_table_cast<get_value_i<unique>>(std::move(i1b)));

     REQUIRE(i1.get_value() == 3.14);
     std::cout << "prototype unique i1: " << i1.get_value() << "\n";
     REQUIRE(get_data(get_virtual_void(i1)) != get_data(get_virtual_void(i0)));
  }
}
