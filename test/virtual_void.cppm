module;

#include <any>
#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

export module m1;
//#define VIRTUAL_VOID_EXPORT export

//#include "virtual_void\any_dispatch\method.h"
//#include "virtual_void\any_dispatch\method_typeid_hash.h"
//#include "virtual_void\class_hierarchy\class_hierarchy.h"
//#include "virtual_void\data\decorated_data.h"
//#include "virtual_void\data\decorated_ptr.h"
//#include "virtual_void\data\decorated_ptr_trait.h"
//#include "virtual_void\data\erased_shared_const.h"
//#include "virtual_void\data\erased_shared_const_ptr.h"
//#include "virtual_void\data\erased_std_ptr_trait.h"
//#include "virtual_void\data\erased_unique.h"
//#include "virtual_void\data\erased_unique_ptr.h"
//#include "virtual_void\data\erased_value.h"
//#include "virtual_void\data\has_m_table\m_table.h"
//#include "virtual_void\data\has_m_table\meta.h"
//#include "virtual_void\data\has_m_table\observer.h"
//#include "virtual_void\data\has_m_table\shared_const.h"
//#include "virtual_void\data\has_m_table\shared_const_ptr.h"
//#include "virtual_void\data\has_m_table\unique.h"
//#include "virtual_void\data\has_m_table\unique_ptr.h"
//#include "virtual_void\data\has_m_table\value.h"
//#include "virtual_void\data\has_no_meta\meta.h"
//#include "virtual_void\data\has_no_meta\observer.h"
//#include "virtual_void\data\has_no_meta\shared_const.h"
//#include "virtual_void\data\has_no_meta\shared_const_ptr.h"
//#include "virtual_void\data\has_no_meta\unique.h"
//#include "virtual_void\data\has_no_meta\unique_ptr.h"
//#include "virtual_void\data\has_no_meta\value.h"
//#include "virtual_void\data\has_type_info\meta.h"
//#include "virtual_void\data\has_type_info\observer.h"
//#include "virtual_void\data\has_type_info\shared_const.h"
//#include "virtual_void\data\has_type_info\shared_const_ptr.h"
//#include "virtual_void\data\has_type_info\unique.h"
//#include "virtual_void\data\has_type_info\unique_ptr.h"
//#include "virtual_void\data\has_type_info\value.h"
//#include "virtual_void\data\make_shared_const_decorated_data.h"
//#include "virtual_void\data\make_unique_decorated_data.h"
//#include "virtual_void\data\make_value_decorated_data.h"
//#include "virtual_void\data\observer.h"
//#include "virtual_void\data\observer_trait.h"
//#include "virtual_void\data\shared_const_ptr.h"
//#include "virtual_void\data\shared_const_ptr_trait.h"
//#include "virtual_void\data\shared_const_trait.h"
//#include "virtual_void\data\unique_ptr.h"
//#include "virtual_void\data\unique_ptr_trait.h"
//#include "virtual_void\data\unique_trait.h"
//#include "virtual_void\data\value_trait.h"
//#include "virtual_void\interface\base.h"
//#include "virtual_void\interface\call_operator.h"
//#include "virtual_void\open_method\algorithm.h"
//#include "virtual_void\open_method\domain.h"
//#include "virtual_void\open_method\table.h"
//#include "virtual_void\open_method\via_m_table\declare.h"
//#include "virtual_void\open_method\via_type_info\declaration_base.h"
//#include "virtual_void\open_method\via_type_info\declare.h"
//#include "virtual_void\open_method\via_type_info\factory.h"
//#include "virtual_void\open_object\members.h"
//#include "virtual_void\perfect_typeid_hash\index_table.h"
//#include "virtual_void\typeid_cast\cast.h"
//#include "virtual_void\utillities\ensure_function_ptr.h"
//#include "virtual_void\utillities\overload.h"
//#include "virtual_void\utillities\type_list.h"
//#include "virtual_void\virtual_void.h"
//
//export namespace virtual_void_export = virtual_void;
//
export std::string hello_world() { return "hello world"; }