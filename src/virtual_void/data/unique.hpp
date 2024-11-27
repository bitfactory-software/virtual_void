#pragma once

#include "erased_unique_ptr.hpp"
#include "decorated_data.hpp"

namespace virtual_void::data {
template <typename META>
using unique = erased_unique_ptr<decoration_base<META>>;
}  // namespace virtual_void::data
