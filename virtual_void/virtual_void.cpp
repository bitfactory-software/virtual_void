#include <any>
#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <random>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <virtual_void/any_dispatch/method.hpp>
#include <virtual_void/any_dispatch/method_typeid_hash.hpp>
#include <virtual_void/meta/class.hpp>
#include <virtual_void/data/copy_convert.hpp>
#include <virtual_void/data/decorated_data.hpp>
#include <virtual_void/data/decorated_ptr.hpp>
#include <virtual_void/data/decorated_ptr_trait.hpp>
#include <virtual_void/data/erased_std_ptr_trait.hpp>
#include <virtual_void/data/erased_unique_ptr.hpp>
#include <virtual_void/data/erased_value.hpp>
#include <virtual_void/data/has_i_table/meta.hpp>
#include <virtual_void/data/has_i_table/observer.hpp>
#include <virtual_void/data/has_i_table/shared_const.hpp>
#include <virtual_void/data/has_i_table/shared_const_ptr.hpp>
#include <virtual_void/data/has_i_table/unique.hpp>
#include <virtual_void/data/has_i_table/unique_ptr.hpp>
#include <virtual_void/data/has_i_table/value.hpp>
#include <virtual_void/data/has_m_table/m_table.hpp>
#include <virtual_void/data/has_m_table/meta.hpp>
#include <virtual_void/data/has_m_table/observer.hpp>
#include <virtual_void/data/has_m_table/shared_const.hpp>
#include <virtual_void/data/has_m_table/shared_const_ptr.hpp>
#include <virtual_void/data/has_m_table/unique.hpp>
#include <virtual_void/data/has_m_table/unique_ptr.hpp>
#include <virtual_void/data/has_m_table/value.hpp>
#include <virtual_void/data/has_no_meta/meta.hpp>
#include <virtual_void/data/has_no_meta/observer.hpp>
#include <virtual_void/data/has_no_meta/shared_const.hpp>
#include <virtual_void/data/has_no_meta/shared_const_ptr.hpp>
#include <virtual_void/data/has_no_meta/unique.hpp>
#include <virtual_void/data/has_no_meta/unique_ptr.hpp>
#include <virtual_void/data/has_no_meta/value.hpp>
#include <virtual_void/data/has_type_info/meta.hpp>
#include <virtual_void/data/has_type_info/observer.hpp>
#include <virtual_void/data/has_type_info/shared_const.hpp>
#include <virtual_void/data/has_type_info/shared_const_ptr.hpp>
#include <virtual_void/data/has_type_info/unique.hpp>
#include <virtual_void/data/has_type_info/unique_ptr.hpp>
#include <virtual_void/data/has_type_info/value.hpp>
#include <virtual_void/data/make_shared_const_decorated_data.hpp>
#include <virtual_void/data/make_unique_decorated_data.hpp>
#include <virtual_void/data/make_value_decorated_data.hpp>
#include <virtual_void/data/move_convert.hpp>
#include <virtual_void/data/observer.hpp>
#include <virtual_void/data/observer_trait.hpp>
#include <virtual_void/data/shared_const.hpp>
#include <virtual_void/data/shared_const_ptr.hpp>
#include <virtual_void/data/shared_const_ptr_trait.hpp>
#include <virtual_void/data/shared_const_trait.hpp>
#include <virtual_void/data/unique.hpp>
#include <virtual_void/data/unique_ptr.hpp>
#include <virtual_void/data/unique_ptr_trait.hpp>
#include <virtual_void/data/unique_trait.hpp>
#include <virtual_void/data/value_trait.hpp>
#include <virtual_void/interface/base.hpp>
#include <virtual_void/interface/call_operator.hpp>
#include <virtual_void/interface/conversion.hpp>
#include <virtual_void/meta/i_table.hpp>
#include <virtual_void/interface/ostream_shift_left_operator.hpp>
#include <virtual_void/interface/subscript_operator.hpp>
#include <virtual_void/open_method/algorithm.hpp>
#include <virtual_void/open_method/domain.hpp>
#include <virtual_void/open_method/table.hpp>
#include <virtual_void/open_method/via_m_table/declare.hpp>
#include <virtual_void/open_method/via_type_info/declaration_base.hpp>
#include <virtual_void/open_method/via_type_info/declare.hpp>
#include <virtual_void/open_method/via_type_info/factory.hpp>
#include <virtual_void/open_object/members.hpp>
#include <virtual_void/perfect_typeid_hash/index_table.hpp>
#include <virtual_void/typeid_cast/cast.hpp>
#include <virtual_void/utillities/ensure_function_ptr.hpp>
#include <virtual_void/utillities/overload.hpp>
#include <virtual_void/utillities/type_list.hpp>
#include <virtual_void/virtual_void.hpp>
