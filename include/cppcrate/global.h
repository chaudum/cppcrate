/*
 * Copyright 2017 Lorenz Haas <lorenz.haas@histomatics.de>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#if defined(_WIN32)
#if defined(CppCrate_EXPORTS)
#define CPPCRATE_EXPORT __declspec(dllexport)
#else
#define CPPCRATE_EXPORT __declspec(dllimport)
#endif
#else
#define CPPCRATE_EXPORT
#endif

#ifdef ENABLE_CPP11_SUPPORT
#include <cstdint>
using std::int16_t;
using std::int32_t;
using std::int64_t;
#define CPPCRATE_NULLPTR nullptr
#define CPPCRATE_TO_STRING(x) std::to_string(x)
#define CPPCRATE_PIMPL_DECLARE_ALL(Class) \
  CPPCRATE_PIMPL_DECLARE_PRIVATE(Class)   \
  CPPCRATE_PIMPL_DECLARE_COPY(Class)      \
  CPPCRATE_PIMPL_DECLARE_MOVE(Class)      \
  CPPCRATE_PIMPL_DECLARE_COMPARISON(Class)
#define CPPCRATE_PIMPL_IMPLEMENT_ALL(Class) \
  CPPCRATE_PIMPL_IMPLEMENT_PRIVATE(Class)   \
  CPPCRATE_PIMPL_IMPLEMENT_COPY(Class)      \
  CPPCRATE_PIMPL_IMPLEMENT_MOVE(Class)      \
  CPPCRATE_PIMPL_IMPLEMENT_COMPARISON(Class)
#else
#include <stdint.h>
#define CPPCRATE_NULLPTR 0
#include <sstream>
namespace CppCrate {
template <class T>
inline std::string to_string(const T &t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}
}
#define CPPCRATE_TO_STRING(x) CppCrate::to_string(x)
#define CPPCRATE_PIMPL_DECLARE_ALL(Class) \
  CPPCRATE_PIMPL_DECLARE_PRIVATE(Class)   \
  CPPCRATE_PIMPL_DECLARE_COPY(Class)      \
  CPPCRATE_PIMPL_DECLARE_COMPARISON(Class)
#define CPPCRATE_PIMPL_IMPLEMENT_ALL(Class) \
  CPPCRATE_PIMPL_IMPLEMENT_PRIVATE(Class)   \
  CPPCRATE_PIMPL_IMPLEMENT_COPY(Class)      \
  CPPCRATE_PIMPL_IMPLEMENT_COMPARISON(Class)
#endif

#define CPPCRATE_PIMPL_DECLARE_PRIVATE(Class) \
 public:                                      \
  ~Class();                                   \
                                              \
 private:                                     \
  class Private;                              \
  Private *p;

#define CPPCRATE_PIMPL_IMPLEMENT_PRIVATE(Class) \
  Class::~Class() { delete p; }

#define CPPCRATE_PIMPL_DECLARE_COPY(Class) \
 public:                                   \
  Class(const Class &other);               \
  Class &operator=(const Class &other);    \
                                           \
 private:

#define CPPCRATE_PIMPL_IMPLEMENT_COPY(Class)                     \
  Class::Class(const Class &other) : p(new Private(*other.p)) {} \
  Class &Class::operator=(const Class &other) {                  \
    if (this != &other) {                                        \
      delete p;                                                  \
      p = new Private(*other.p);                                 \
    }                                                            \
    return *this;                                                \
  }

#define CPPCRATE_PIMPL_DECLARE_MOVE(Class) \
 public:                                   \
  Class(Class &&other);                    \
  Class &operator=(Class &&other);         \
                                           \
 private:

#define CPPCRATE_PIMPL_IMPLEMENT_MOVE(Class)                                          \
  Class::Class(Class &&other) : p(std::move(other.p)) { other.p = CPPCRATE_NULLPTR; } \
  Class &Class::operator=(Class &&other) {                                            \
    if (this != &other) {                                                             \
      delete p;                                                                       \
      p = std::move(other.p);                                                         \
      other.p = CPPCRATE_NULLPTR;                                                     \
    }                                                                                 \
    return *this;                                                                     \
  }

#define CPPCRATE_PIMPL_DECLARE_COMPARISON(Class) \
 public:                                         \
  bool operator==(const Class &other) const;     \
  bool operator!=(const Class &other) const;     \
                                                 \
 private:

#define CPPCRATE_PIMPL_IMPLEMENT_COMPARISON(Class)                            \
  bool Class::operator==(const Class &other) const { return *p == *other.p; } \
  bool Class::operator!=(const Class &other) const { return !(*this == other); }
