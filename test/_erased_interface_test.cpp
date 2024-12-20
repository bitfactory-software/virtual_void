#include <catch.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <virtual_void/data/has_m_table/shared_const.hpp>
#include <virtual_void/data/has_no_meta/observer.hpp>
#include <virtual_void/data/has_no_meta/value.hpp>
#include <virtual_void/data/has_type_info/unique.hpp>
#include <virtual_void/interface/call_operator.hpp>
#include <virtual_void/interface/declare_macro.hpp>
#include <virtual_void/meta/i_table.hpp>

using namespace Catch::Matchers;

using namespace virtual_void;

const double M_PI = 3.14;

struct position {
  float x, y;
};

namespace {
VV_INTERFACE(shape_base1, (VV_CONST_METHOD(void, draw, position)))

VV_INTERFACE_(shape_base, shape_base1, (VV_CONST_METHOD(int, count_sides)))

VV_INTERFACE_(shape_d_i, shape_base,
              (VV_CONST_METHOD(double, area),
               VV_CONST_METHOD(double, perimeter)))

VV_INTERFACE_(shape_i, shape_base1,
              (VV_CONST_METHOD(int, count_sides), VV_CONST_METHOD(double, area),
               VV_CONST_METHOD(double, perimeter)))
}  // namespace

VV_INTERFACE_META(, shape_base1)
VV_INTERFACE_META(, shape_base)
VV_INTERFACE_META(, shape_d_i)
VV_INTERFACE_META_IMPEMENTATION(shape_base1)
VV_INTERFACE_META_IMPEMENTATION(shape_base)
VV_INTERFACE_META_IMPEMENTATION(shape_d_i)

struct circle {
  double radius;
  void draw(position p) const {
    std::cout << " A Circle Is Recorded At " << p.x << " " << p.y << std::endl;
  }
  int count_sides() const { return 1; }
  double area() const { return radius * radius * M_PI; }
  double circumference() const { return radius * 2.0f * M_PI; }
  double perimeter() const { return circumference(); }
  std::string operator()(const std::string& x) const { return x + "circle"; }
};

template <>
struct virtual_void::meta::i_table_of<circle>
    : virtual_void::meta::i_table_implementation_of<circle> {};
virtual_void::meta::is_a<circle, shape_d_i_v_table> __;

TEST_CASE("class is_a interface") {
  using namespace virtual_void;
  using namespace virtual_void::meta;

  auto& unspecified = archetype_instance<archetype_unspecified>();
  REQUIRE(&get_i_table_of<circle>().get_archetype() == &unspecified);
  auto& shape_d_i_meta = interface_meta_for<shape_d_i_v_table>();
  auto shape_d_i_index = shape_d_i_meta.i_table_index(unspecified);
  REQUIRE(shape_d_i_index >= 0);
  static_assert(
      std::same_as<shape_d_i_v_table::v_table_base_t, shape_base_v_table>);
  auto& shape_base_meta = interface_meta_for<shape_base_v_table>();
  auto shape_base_index = shape_base_meta.i_table_index(unspecified);
  REQUIRE(shape_base_index == shape_d_i_index);
  static_assert(
      std::same_as<shape_base_v_table::v_table_base_t, shape_base1_v_table>);
  auto& shape_base1_meta = interface_meta_for<shape_base1_v_table>();
  auto shape_base1_index = shape_base1_meta.i_table_index(unspecified);
  REQUIRE(shape_base1_index == shape_base_index);
}

using shape_vv = shape_i<data::has_m_table::shared_const>;

using shape_base_v = shape_base<data::has_no_meta::const_observer>;

using shape =
    interface::call_operator<data::has_no_meta::const_observer,
                             std::string(std::string), const_, shape_d_i>;
using shapeX = shape_d_i<data::has_no_meta::const_observer>;
using shapeXX = shape_d_i<data::has_no_meta::const_observer>;

using full_shape_observer = shape_i<data::has_no_meta::mutable_observer>;

template <>
struct shape_base1_v_table_map<const circle> {
  auto draw(circle const* x, position p) const {
    std::cout << " A Circle Is Recorded VIA circle_shape_base1_v_table_map At "
              << p.x << " " << p.y << std::endl;
  }
};

template <>
struct shape_i_v_table_map<const circle>
    : shape_i_default_v_table_map<circle const> {
  auto draw(circle const* x, position p) const {
    std::cout << " A Circle Is Recorded VIA circle_shape_i_v_table_map At "
              << p.x << " " << p.y << std::endl;
  }
};

struct square {
  int w;
  void draw(position p) const {
    std::cout << " A Square Is Recorded At " << p.x << " " << p.y << std::endl;
  }
  int count_sides() const { return 4; }
  double area() const { return w * w; }
  double perimeter() const { return w * 4; }
  std::string operator()(const std::string& x) const { return x + "square"; }
};
struct rectangle {
  int w, h;
  void draw(position p) const {
    std::cout << " A Rectangle Is Recorded At " << p.x << " " << p.y
              << std::endl;
  }
  int count_sides() const { return 4; }
  double area() const { return w * h; }
  double perimeter() const { return w + w + h + h; }
  std::string operator()(const std::string& x) const { return x + "rectangle"; }
};
struct regular_polygon {
  int sides;
  double side_length;
  void draw(position p) const {
    std::cout << " A Polygon Is Recorded At " << p.x << ", " << p.y
              << std::endl;
  }
  int count_sides() const { return sides; }
  double apothem() const { return (side_length / 2) / std::tan(M_PI / sides); }
  double radius() const { return (side_length / 2) / std::sin(M_PI / sides); }
  double perimeter() const { return sides * side_length; }
  double area() const { return (perimeter() * apothem()) / 2; }
  std::string operator()(const std::string& x) const {
    return x + "regular_polygon";
  }
};

void print_shape_(const auto s) {
  s.draw({4.0, 5.0});
  std::cout << "Shape Number Of Sides: " << s.count_sides() << std::endl;
  std::cout << "Shape Perimeter: " << s.perimeter() << std::endl;
  std::cout << "Shape Area: " << s.area() << std::endl;
}
void print_shape_vv(const shape_vv s) { print_shape_(s); }
void print_shape(const shape s) {
  print_shape_(s);
  std::cout << s("Shape type = ") << std::endl;
}
void print_shape_f(const full_shape_observer s) { print_shape_(s); }

// using shape_double_base_error = shape_d_i<
// data::has_no_meta::const_observer, bases<
// shape_base, shape_base > >; //should not compile! void
// should_not_compile(shape_double_base_error s) {}//should not compile!

TEST_CASE("dynamic v_table const_observer") {
  using namespace virtual_void;
  using namespace virtual_void::meta;

  circle c{12.3};
  square s{32};
  rectangle r{12, 9};
  regular_polygon p{4, 32};

  std::cout << "print_shape *******************************" << std::endl;

  print_shape(c);
  print_shape(s);
  print_shape(r);
  print_shape(p);

  using erased_const_observer = data::has_no_meta::const_observer;
  static_assert(
      std::is_base_of_v<interface::base<erased_const_observer>, shape>);
  static_assert(std::is_base_of_v<shape_base_v, shape>);
  static_assert(std::derived_from<shape, shape_base_v>);
  shape shape_circle{circle{33.3}};
  shapeX shape_circleX{circle{33.3}};

  data::has_no_meta::const_observer o1 =
      virtual_void::erased<data::has_no_meta::const_observer>(c);
  data::has_no_meta::const_observer o2 = o1;

  {
    using shape_base1_const_observer =
        shape_base1<data::has_no_meta::const_observer>;
    shape_base1_const_observer sb1;
    shape_base1_const_observer sb2{c};
    sb1 = sb2;
  }
  {
    using shape_base1_mutable_observer =
        shape_base1<data::has_no_meta::mutable_observer>;
    shape_base1_mutable_observer sb1;
    shape_base1_mutable_observer sb2{c};
    sb1 = sb2;
  }
  {
    using shape_base1_mutable_observer =
        shape_base1<data::has_no_meta::mutable_observer>;
    shape_base1_mutable_observer sb1{c};
    shape_base1_mutable_observer sb2{std::move(sb1)};
  }

  //    base< void* > base_v = shape_circle; ->
  //    v_table_cast may not compile!
  interface::base<data::has_no_meta::const_observer> base_shape = shape_circle;
  interface::base<data::has_no_meta::const_observer> base_shapeX =
      shape_circleX;

  REQUIRE(is_derived_from<shape>(base_shape));
  REQUIRE(is_derived_from<shapeX>(base_shape));
  REQUIRE(!is_derived_from<shape>(shape_circleX));
  REQUIRE(is_derived_from<shapeX>(shape_circleX));
  static_assert(std::derived_from<shape, shape_base_v>);
  static_assert(std::derived_from<shapeX, shape_base_v>);
  REQUIRE(interface::v_table_cast<shapeX>(base_shape));
  REQUIRE(!interface::v_table_cast<shape>(shape_circleX));
  {
    shape upcasted_shape = interface::unchecked_v_table_cast<shape>(base_shape);
    print_shape(upcasted_shape);
  }

  shape_base_v shape_circle_base = shape_circle;
  {
    shape shape_is_circle =
        interface::unchecked_v_table_cast<shape>(shape_circle_base);
    print_shape(shape_is_circle);
  }
  {
    auto shape_is_circle = interface::v_table_cast<shape>(shape_circle_base);
    REQUIRE(shape_is_circle);
    print_shape(*shape_is_circle);
  }

  print_shape_f(full_shape_observer{p});
}

TEST_CASE("dynamic interface m_table::shared_const") {
  circle c{12.3};
  square s{32};
  rectangle r{12, 9};
  regular_polygon p{4, 32};
  std::cout << "print_shape_vv ********************************" << std::endl;

  data::has_m_table::typed_shared_const<circle> sc{c};
  auto& c1 = sc;
  REQUIRE_THAT(c1->perimeter(), WithinAbs(77.2, 77.3));
  static_assert(std::same_as<
                data::has_m_table::typed_shared_const<circle>::virtual_void_t,
                data::has_m_table::shared_const>);
  static_assert(is_virtual_typed<decltype(sc)>);
  shape_vv circle_shape_vv{sc};
  auto unerased_circle =
      unerase_cast<circle const>(get_virtual_void(circle_shape_vv));
  REQUIRE_THAT(unerased_circle->perimeter(), WithinAbs(77.2, 77.3));
  auto x = circle_shape_vv;
  {
    auto perimeter = circle_shape_vv.perimeter();
    REQUIRE_THAT(perimeter, WithinAbs(77.2, 77.3));
  }
  print_shape_vv(sc);
  print_shape_vv(circle_shape_vv);
  print_shape_vv(c);
  print_shape_vv(s);
  print_shape_vv(r);
  print_shape_vv(p);
}

TEST_CASE("dynamic interface has_type_info::unique") {
  circle c{12.3};

  using shape_unique = shape_i<data::has_type_info::unique>;
  shape_unique s1{c};

  REQUIRE_THAT(s1.perimeter(), WithinAbs(77.2, 77.3));
  auto unerased_circle = unerase_cast<circle const>(get_virtual_void(s1));
  REQUIRE_THAT(unerased_circle->perimeter(), WithinAbs(77.2, 77.3));
  print_shape_vv(std::move(s1));
}

TEST_CASE("base") {
  using namespace virtual_void;
  using namespace virtual_void;
  using value = data::has_no_meta::value;

  using value_base = interface::base<value>;

  struct x_t {
    std::string s_;
  };
  {
    x_t a{"hallo"};
    auto e = erased<value>(a);
    REQUIRE(unerase_cast<x_t>(e)->s_ == "hallo");
  }
  {
    x_t a{"hallo"};
    value_base vb(a);
    REQUIRE(unerase_cast<x_t>(get_virtual_void(vb))->s_ == "hallo");
  }
}