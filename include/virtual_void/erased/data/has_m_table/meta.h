#pragma once

#include "../holder.h"
#include "m_table.h"

namespace virtual_void::erased::data::has_m_table {

struct meta {
  m_table_t* m_table_ = nullptr;

  const auto* get_meta() const { return this; }

  template <typename T>
  meta(std::in_place_type_t<T>) : m_table_(m_table_of<std::decay_t<T>>()) {}

  meta() noexcept = default;
  meta(const meta&) = default;
  meta(meta&) = default;
  meta(meta&& rhs) noexcept { swap(*this, rhs); }
  meta& operator=(meta&& rhs) noexcept {
    meta destroy_this{};
    swap(*this, destroy_this);
    swap(*this, rhs);
    return *this;
  }
  friend void swap(meta& lhs, meta& rhs) noexcept {
    using namespace std;
    swap(lhs.m_table_, rhs.m_table_);
  }

  type_info_ptr type_info() const { return &get_m_table()->type(); }
  m_table_t* get_m_table() const { return m_table_; }
  DATA_ALIGNED_DESRTUCTOR_VIRTUAL ~meta() = default;
};

}  // namespace virtual_void::erased::data::has_m_table