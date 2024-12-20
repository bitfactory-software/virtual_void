#pragma once

#include <virtual_void/data/observer.hpp>
#include <virtual_void/data/observer_trait.hpp>
#include <virtual_void/data/has_no_meta/meta.hpp>

namespace virtual_void::data::has_no_meta {

template <typename VV_VOID>
using observer = data::observer<VV_VOID, meta>;
}

namespace virtual_void {
template <typename VV_VOID>
struct virtual_void_trait<data::has_no_meta::observer<VV_VOID>>
    : observer_trait<data::has_no_meta::observer<VV_VOID>, VV_VOID,
                     data::has_no_meta::meta> {};
}  // namespace virtual_void

namespace virtual_void::data::has_no_meta {
using const_observer = observer<void const*>;
using mutable_observer = observer<void*>;
template <typename V, typename VV_VOID>
using typed_observer = virtual_typed<V, observer<VV_VOID>>;
template <typename V>
using typed_const_observer = typed_observer<V, void const*>;
template <typename V>
using typed_mutable_observer = typed_observer<V, void*>;
static_assert(virtual_void_trait<const_observer>::is_const);
static_assert(!virtual_void_trait<mutable_observer>::is_const);
// static_assert(std::is_same_v<const_observer::void_t,void const*>);
// static_assert(std::is_same_v<observer_trait<const_observer::data_t>::void_t,void
// const*>); static_assert(is_const_void<void const*>::value);
// static_assert(is_const_void<observer_trait<const_observer::data_t>::void_t>::value);
static_assert(is_virtual_void<const_observer>);
static_assert(is_virtual_void<mutable_observer>);
static_assert(is_virtual_void<mutable_observer>);
static_assert(is_virtual_void<const_observer>);
static_assert(is_virtual_typed<typed_const_observer<int>>);
static_assert(is_virtual_typed<typed_mutable_observer<int>>);
}  // namespace virtual_void::data::has_no_meta
