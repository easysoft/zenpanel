/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef PTR_VECTOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66
#define PTR_VECTOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66

#if defined(_MSC_VER) ||                                            \
    (defined(__GNUC__) && (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
     (__GNUC__ >= 4))  // GCC supports "pragma once" correctly since 3.4
#pragma once
#endif

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <vector>

#include "yaml-cpp/noncopyable.h"

namespace YAML {

// TODO: This class is no longer needed
template <typename T>
class ptr_vector : private YAML::noncopyable {
 public:
  ptr_vector() {}

  void clear() { m_data.clear(); }

  std::size_t size() const { return m_data.size(); }
  bool empty() const { return m_data.empty(); }

  void push_back(std::unique_ptr<T>&& t) { m_data.push_back(std::move(t)); }
  T& operator[](std::size_t i) { return *m_data[i]; }
  const T& operator[](std::size_t i) const { return *m_data[i]; }

  T& back() { return *(m_data.back().get()); }

  const T& back() const { return *(m_data.back().get()); }

 private:
  std::vector<std::unique_ptr<T>> m_data;
};
}

#endif  // PTR_VECTOR_H_62B23520_7C8E_11DE_8A39_0800200C9A66
