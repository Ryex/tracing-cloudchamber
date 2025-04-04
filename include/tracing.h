#pragma once

#include <iostream>
#include <string_view>
#include <vector>

#include "tracing-cloudchamber/src/lib.rs.h"

namespace cloudchamber {

namespace level {
static const Level ERROR = Level{LevelValue::ERROR};
static const Level WARN = Level{LevelValue::WARN};
static const Level INFO = Level{LevelValue::INFO};
static const Level DEBUG = Level{LevelValue::DEBUG};
static const Level TRACE = Level{LevelValue::TRACE};
} // namespace level
namespace kind {
static const Kind EVENT = Kind{KindValue::EVENT};
static const Kind SPAN = Kind{KindValue::SPAN};
static const Kind HINT = Kind{KindValue::HINT};
} // namespace kind

namespace detail {

template <typename T>
constexpr std::string_view __QUALIFIED_FUNCTION(T &&name) {
  std::string_view name_s(name);
  auto end = name_s.rfind(")");
  while (end != std::string::npos) {
    auto end_c = name_s.find(">", end);
    if (end_c == std::string::npos)
      break;
    end = end_c + 1;
  }
  if (end == std::string::npos)
    return name;

  auto start = name_s.find("(");
  while (start != std::string::npos) {
    auto start_c = name_s.rfind("<", start);
    if (start_c == std::string::npos)
      break;
    start = start_c - 1;
  }
  if (start != std::string::npos) {
    start = name_s.rfind(" ", start);
  }
  if (start == std::string::npos)
    start = 0;
  else
    start += 1;
  auto qual_name = name_s.substr(start, end - start + 1);
  return qual_name;
}

} // namespace detail

} // namespace cloudchamber

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

void print_type() {

  static std::array<const ::rust::Str, 3> __FIELDS = {};
  static ::cloudchamber::Metadata __META{
      ::rust::Str("print_type.event"),
      ::rust::Str(
          ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__)
              .cbegin()),
      ::cloudchamber::level::INFO,
      ::rust::Str(__FILE__),
      int32_t(__LINE__),
      ::rust::Slice<const ::rust::Str>(__FIELDS),
      nullptr,
      ::cloudchamber::Kind{::cloudchamber::KindValue::EVENT}};
  static ::cloudchamber::Callsite __CALLSITE(__META);
  __META.callsite = &__CALLSITE;
  constexpr std::string_view _func =
      ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__);
  std::cout << "raw" << _func << std::endl;
}

#ifndef tchEvent
#define tchEvent(level, name)                                                  \
  static std::vector<::rust::Str> __FIELDS = {};                               \
  std::cout << "Callsite was created" << std::endl;                            \
  static ::cloudchamber::Metadata __META{                                      \
      ::rust::Str(name),                                                       \
      ::rust::Str(                                                             \
          ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__)    \
              .cbegin()),                                                      \
      level,                                                                   \
      ::rust::Str(__FILE__),                                                   \
      int32_t(__LINE__),                                                       \
      ::rust::Slice<const ::rust::Str>(__FIELDS),                              \
      nullptr,                                                                 \
      ::cloudchamber::Kind{::cloudchamber::KindValue::EVENT}};                 \
  static ::cloudchamber::Callsite __CALLSITE(__META);                          \
  __META.callsite = &__CALLSITE;                                               \
  bool __CALLSITE_ENABLED = level.is_enabled();                                \
  if (__CALLSITE_ENABLED) {                                                    \
    std::cout << "Callsite was high enough level" << std::endl;                \
    __CALLSITE_ENABLED = __CALLSITE.is_enabled();                              \
  }                                                                            \
  if (__CALLSITE_ENABLED) {                                                    \
    /* push fields */                                                          \
    std::cout << "Callsite was enabled" << std::endl;                          \
    ::cloudchamber::dispatch_tracing_event(__CALLSITE.get_meta());             \
  }

#endif
