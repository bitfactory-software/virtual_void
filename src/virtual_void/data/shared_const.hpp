#pragma once

#include <memory>

#include "decorated_data.hpp"

namespace virtual_void::data {
template <typename META>
using shared_const = std::shared_ptr<decoration_base<META> const>;
}  // namespace virtual_void::data