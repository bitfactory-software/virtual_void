#pragma once

#undef interface

#include <virtual_void/data/has_no_meta/observer.hpp>
#include <virtual_void/data/has_type_info/observer.hpp>
#include <virtual_void/data/has_type_info/shared_const.hpp>
#include <virtual_void/data/has_type_info/unique.hpp>
#include <virtual_void/data/has_type_info/value.hpp>
#include <virtual_void/interface/conversions_macros.hpp>

VV_FIND_COPY_DECLARE(has_no_meta::const_observer,
                     has_type_info::const_observer)
VV_FIND_COPY_DECLARE(has_no_meta::mutable_observer,
                     has_type_info::mutable_observer)
VV_FIND_COPY_DECLARE(has_no_meta::const_observer, has_type_info::shared_const)
VV_FIND_COPY_DECLARE(has_no_meta::mutable_observer, has_type_info::unique)

